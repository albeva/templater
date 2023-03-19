//
// Created by Albert on 06/03/2023.
//
#include "Source.hpp"
#include <fstream>
using support::Source;
using namespace std::literals;

Source::Source(const std::filesystem::path& path)
    : m_name { path.generic_string() }
{
    if (!std::filesystem::exists(path)) {
        throw SourceException("File '" + m_name + "' not found");
    }

    std::ifstream file { path, std::ios::binary | std::ios::in };
    if (!file.is_open()) {
        throw SourceException("Failed to open '" + m_name + "'");
    }

    m_source = std::string { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

Source::Source(std::string name, std::string source) noexcept
    : m_name { std::move(name) }
    , m_source { std::move(source) }
{
}

auto Source::getPosition(SourceLoc loc) const -> SourcePos
{
    auto range = normalize(loc);

    auto line = std::count(data(), range.first, '\n');
    auto start = std::find(std::make_reverse_iterator(range.first), std::make_reverse_iterator(data()), '\n');

    return {
        static_cast<unsigned>(line + 1),
        static_cast<unsigned>(std::distance(start.base(), range.first) + 1),
        loc.length()
    };
}

auto Source::getLine(std::size_t line) const -> std::string_view
{
    const char* from = getLineStart(line);
    const char* to = std::find_if(from, end(), [](char ch) { return ch == '\r' || ch == '\n'; });
    return { from, to };
}

auto Source::getString(SourceLoc loc) const -> std::string_view
{
    auto range = normalize(loc);
    return { range.first, range.second };
}

auto Source::highlight(SourcePos pos) const -> std::string
{
    return fmt::format(
        "{0}\n{1:>{3}}{2:~>{4}}",
        getLine(pos.getLine()), // 0
        '^',                    // 1
        "",                     // 2
        pos.getCol(),
        pos.getLength() > 1 ? pos.getLength() - 1 : 0);
}

auto Source::normalize(const SourceLoc& loc) const -> Range
{
    const char* from = data();
    std::advance(from, loc.getStart());

    const char* to = data();
    std::advance(to, loc.getEnd());

    if (from > end() || to > end()) {
        throw SourceException("SourceLoc outside Source range");
    }

    return { from, to };
}

auto Source::getLineStart(size_t line) const -> const char*
{
    if (line == 1) {
        return data();
    }

    const char* start = std::find_if(data(), end(), [&](char ch) {
        if (ch != '\n') {
            return false;
        }
        return --line == 1;
    });
    if (start == end()) {
        throw SourceException(fmt::format("Kube {} us out of Source range", line));
    }
    std::advance(start, 1); // skip \n
    return start;
}

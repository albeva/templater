//
// Created by Albert on 06/03/2023.
//
#include "Source.hpp"
#include <fstream>
using namespace templater;

Source::Source(const std::filesystem::path& path)
    : m_name { path.string() }
{
    if (!std::filesystem::exists(path)) {
        throw SourceException("File '" + m_name + "' not found");
    }

    std::ifstream file { path, std::ios::binary | std::ios::in };
    if (!file.is_open()) {
        throw SourceException("Failed to open '"s + m_name + "'");
    }

    m_source = std::string { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

Source::Source(std::string name, std::string source)
    : m_name { std::move(name) }
    , m_source { std::move(source) }
{
}

auto Source::getPosition(SourceLoc loc) const -> Source::Position
{
    verify(loc);

    auto line = std::count(data(), loc.getStart(), '\n');
    auto start = std::find(std::make_reverse_iterator(loc.getStart()), std::make_reverse_iterator(data()), '\n');
    return {
        .line = static_cast<unsigned>(line + 1),
        .col = static_cast<unsigned>(std::distance(start.base(), loc.getStart()) + 1),
        .len = loc.length()
    };
}

auto Source::getString(std::size_t line) const -> std::string_view
{
    const char* from = getLineStart(line);
    const char* to = std::find_if(from, end(), [](char ch) { return ch == '\r' || ch == '\n'; });
    return { from, to };
}

auto Source::getString(SourceLoc loc) const -> std::string_view
{
    verify(loc);
    return { loc.getStart(), loc.getEnd() };
}

auto Source::highlight(Position pos) const -> std::string
{
    return fmt::format(
        "{0}\n{1:>{3}}{2:~>{4}}",
        getString(pos.line), // 0
        '^', // 1
        "", // 2
        pos.col,
        pos.len > 1 ? pos.len - 1 : 0);
}

void Source::verify(const SourceLoc& loc) const
{
    const char* start = data();

    auto invalid = [&](const char* ptr) {
        return ptr < start || ptr > end();
    };

    if (invalid(loc.getStart()) || invalid(loc.getEnd())) {
        throw SourceException("SourceLoc outside Source range");
    }
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
        std::stringstream is;
        is << "Line " << line << "out of Source range";
        throw SourceException(is.str());
    }
    std::advance(start, 1);
    return start;
}

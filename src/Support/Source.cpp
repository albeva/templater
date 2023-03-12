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

auto Source::getLineAndCol(SourceLoc loc) const -> Source::LineAndCol
{
    verify(loc);
    auto line = std::count(data(), loc.start, '\n');
    auto start = std::find(std::make_reverse_iterator(loc.start), std::make_reverse_iterator(data()), '\n');
    return {
        .line = static_cast<unsigned>(line + 1),
        .col = static_cast<unsigned>(std::distance(start.base(), loc.start) + 1)
    };
}

void Source::verify(const SourceLoc& loc) const
{
    const char* start = data();

    auto invalid = [&](const char* ptr) {
        return ptr < start || ptr > end();
    };

    if (invalid(loc.start) || invalid(loc.end)) {
        throw SourceException("SourceLoc outside Source range");
    }
}

auto Source::getString(std::size_t line) const -> std::string_view
{
    const char* from {};
    if (line == 1) {
        from = data();
    } else {
        from = std::find_if(data(), end(), [&](char ch) {
            if (ch != '\n') {
                return false;
            }
            return --line == 1;
        });
        if (from == end()) {
            throw SourceException(std::format("Line {} out of source range", line));
        }
        std::advance(from, 1);
    }

    const char* to = std::find_if(from, end(), [](char ch) { return ch == '\r' || ch == '\n'; });
    return { from, to };
}

auto Source::getString(SourceLoc loc) const -> std::string_view
{
    verify(loc);
    return { loc.start, loc.end };
}

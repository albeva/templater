//
// Created by Albert on 06/03/2023.
//
#include "Source.hpp"
#include <fstream>
#include <utility>
using namespace templater;

Source::Source(const std::filesystem::path& path)
    : m_name { path.string() }
{
    if (!fs::exists(path)) {
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

std::pair<std::size_t, std::size_t> Source::getLineAndCol(SourceLoc loc) const
{
    auto line = std::count(data(), loc.start, '\n');
    auto lineStart = std::find(
        std::make_reverse_iterator(loc.start),
        std::make_reverse_iterator(data()),
        '\n');
    return {
        line + 1,
        std::distance(lineStart.base(), loc.start) + 1
    };
}

std::string_view Source::getLine(std::size_t line, SourceLoc highlight) const
{
    (void)line;
    (void)highlight;
    (void)this;
    return "";
}

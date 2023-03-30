//
// Created by Albert on 06/03/2023.
//
#include "Source.hpp"
#include <fstream>
#include <optional>
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

    m_buffer = std::string { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

Source::Source(std::string name, std::string source) noexcept
    : m_name { std::move(name) }
    , m_buffer { std::move(source) }
{
}

auto Source::getPosition(SourceLoc loc) const -> SourcePos
{
    auto range = getRange(loc);

    auto line = std::count(m_buffer.begin(), range.first, '\n');
    auto start = std::find(std::make_reverse_iterator(range.first), std::make_reverse_iterator(m_buffer.begin()), '\n');

    return {
        static_cast<unsigned>(line + 1),
        static_cast<unsigned>(std::distance(start.base(), range.first) + 1),
        loc.length()
    };
}

auto Source::getLine(std::size_t line) const noexcept -> std::optional<std::string_view>
{
    assert(line > 0 && "Line is indexed from 1");
    --line;

    auto start = m_buffer.begin();
    auto iter = start;

    for (; iter != m_buffer.end(); iter++) {
        if (*iter == '\n') {
            if (line == 0) {
                auto prev = std::prev(iter);
                if (*prev == '\r') {
                    iter = prev;
                }
                return { { start, iter } };
            }
            start = iter + 1;
            --line;
        }
    }

    if (line == 0) {
        return { { start, iter } };
    }
    return {};
}

auto Source::highlight(SourcePos pos) const -> std::string
{
    if (auto line = getLine(pos.getLine())) {
        return fmt::format(
            "{0}\n{1:>{3}}{2:~>{4}}",
            line.value(), // 0
            '^',          // 1
            "",           // 2
            pos.getCol(),
            pos.getLength() > 1 ? pos.getLength() - 1 : 0);
    }
    return ""s;
}

auto Source::getRange(const SourceLoc& loc) const -> Range
{
    auto from = m_buffer.begin();
    std::advance(from, loc.getStart());

    auto to = m_buffer.begin();
    std::advance(to, loc.getEnd());

    if (from > to || to > m_buffer.end()) {
        throw SourceException("SourceLoc outside Source range");
    }

    return { from, to };
}

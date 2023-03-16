//
// Created by Albert on 06/03/2023.
//
#pragma once
#include "pch.hpp"
namespace templater::support {

// defines a location range based on start and end index
struct SourceLoc final {
    SourceLoc() noexcept = default;
    SourceLoc(unsigned start, unsigned end) noexcept
        : m_start { start }
        , m_end { end }
    {
    }

    SourceLoc(const SourceLoc& lhs, const SourceLoc& rhs) noexcept
        : m_start { lhs.m_start }
        , m_end { rhs.m_end }
    {
    }

    [[nodiscard]] inline auto getStart() const noexcept { return m_start; }
    [[nodiscard]] inline auto getEnd() const noexcept { return m_end; }
    [[nodiscard]] inline auto length() const noexcept { return m_end - m_start; }

private:
    unsigned m_start = 0, m_end = 0;
};

// defines source position in terms of human friendly line, col and length
struct SourcePos final {
    SourcePos() noexcept = default;
    SourcePos(unsigned line, unsigned col, unsigned len) noexcept
        : m_line(line)
        , m_col(col)
        , m_len(len)
    {
    }

    [[nodiscard]] inline auto getLine() const noexcept { return m_line; }
    [[nodiscard]] inline auto getCol() const noexcept { return m_col; }
    [[nodiscard]] inline auto getLength() const noexcept { return m_len; }

private:
    unsigned m_line = 0, m_col = 0, m_len = 0;
};

} // namespace templater::support

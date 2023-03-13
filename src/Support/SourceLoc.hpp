//
// Created by Albert on 06/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater {

// defines a location range based on start and end index
struct SourceLoc final {
    SourceLoc() = default;

    SourceLoc(unsigned start, unsigned end)
        : m_start { start }
        , m_end { end }
    {
    }

    SourceLoc(const SourceLoc& lhs, const SourceLoc& rhs)
        : m_start { lhs.m_start }
        , m_end { rhs.m_end }
    {
    }

    [[nodiscard]] auto getStart() const { return m_start; }
    [[nodiscard]] auto getEnd() const { return m_end; }
    [[nodiscard]] auto length() const { return m_end - m_start; }

private:
    unsigned m_start = 0, m_end = 0;
};

// defines source position in terms of human friendly line, col and length
struct SourcePos final {
    SourcePos() = default;
    SourcePos(unsigned line, unsigned col, unsigned len)
        : m_line(line)
        , m_col(col)
        , m_len(len)
    {
    }

    [[nodiscard]] auto getLine() const { return m_line; }
    [[nodiscard]] auto getCol() const { return m_col; }
    [[nodiscard]] auto getLength() const { return m_len; }

private:
    unsigned m_line = 0, m_col = 0, m_len = 0;
};

} // namespace templater

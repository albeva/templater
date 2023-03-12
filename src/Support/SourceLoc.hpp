//
// Created by Albert on 06/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater {

struct SourceLoc final {
    SourceLoc()
        : m_start { nullptr }
        , m_end { nullptr }
    {
    }

    SourceLoc(const char* start, const char* end)
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

    [[nodiscard]] auto length() const -> unsigned
    {
        return static_cast<unsigned>(std::distance(m_start, m_end));
    }

private:
    const char *m_start, *m_end;
};

} // namespace templater

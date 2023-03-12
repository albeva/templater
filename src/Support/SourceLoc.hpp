//
// Created by Albert on 06/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater {

struct SourceLoc final {
    SourceLoc()
        : start { nullptr }
        , end { nullptr }
    {
    }

    SourceLoc(const char* start, const char* end)
        : start { start }
        , end { end }
    {
    }

    SourceLoc(const SourceLoc& lhs, const SourceLoc& rhs)
        : start { lhs.start }
        , end { rhs.end }
    {
    }

    const char *start, *end;

    [[nodiscard]] unsigned length() const
    {
        return static_cast<unsigned>(std::distance(start, end));
    }
};

} // namespace templater

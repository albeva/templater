//
// Created by Albert on 06/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater {

struct SourceLoc final {
    const char *start, *end;

    [[nodiscard]] unsigned length() const
    {
        return static_cast<unsigned>(std::distance(start, end));
    }
};

} // namespace templater

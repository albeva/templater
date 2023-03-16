//
// Created by Albert on 15/03/2023.
//
#pragma once
#include "pch.hpp"
namespace templater::support {

struct Separator final {
    constexpr explicit Separator(std::string_view separator) noexcept
        : m_separator(separator)
    {
    }

    constexpr auto operator()() noexcept -> std::string_view
    {
        if (m_first) {
            m_first = false;
            return {};
        }
        return m_separator;
    }

private:
    bool m_first = true;
    std::string_view m_separator;
};

} // namespace templater::support

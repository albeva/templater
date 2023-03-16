//
// Created by Albert on 16/03/2023.
//
#pragma once
#include "pch.hpp"
namespace templater::support {
void printGrid(
    std::ostream& stream,
    const std::vector<std::string>& table,
    size_t cols, std::string_view sep = " ");
} // namespace templater::support

//
// Created by Albert on 16/03/2023.
//
#pragma once
#include "pch.hpp"
namespace support {
void printGrid(
    std::ostream& stream,
    const std::vector<std::string>& table,
    size_t cols);

void printGrid(
    std::ostream& stream,
    const std::vector<std::vector<std::string>>& table);
} // namespace support

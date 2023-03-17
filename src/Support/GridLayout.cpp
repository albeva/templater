//
// Created by Albert on 16/03/2023.
//
#include "GridLayout.hpp"

void support::printGrid(
    std::ostream& stream,
    const std::vector<std::string>& table,
    size_t cols)
{
    // Find column sizes
    std::vector<size_t> widths(cols);
    for (size_t idx = 0; idx < table.size(); idx++) {
        const size_t col = idx % cols;
        widths[col] = std::max(widths[col], table[idx].length());
    }

    // print out
    for (size_t idx = 0; idx < table.size(); idx++) {
        const size_t col = idx % cols;
        if (col == 0) {
            stream << "\n   ";
        }
        fmt::print(stream, " {:<{}}", table[idx], widths[col]);
    }
}

void support::printGrid(
    std::ostream& stream,
    const std::vector<std::vector<std::string>>& table)
{
    // Find column sizes
    std::vector<size_t> widths;
    for (const auto& row : table) {
        for (size_t col = 0; col < row.size(); col++) {
            if (col >= widths.size()) {
                widths.push_back(0);
            }
            widths[col] = std::max(widths[col], row[col].length());
        }
    }

    // Print
    for (const auto& row : table) {
        for (size_t col = 0; col < row.size(); col++) {
            if (col == 0) {
                fmt::print(stream, "\n   ");
            }
            fmt::print(stream, " {:<{}}", row[col], widths[col]);
        }
    }
}

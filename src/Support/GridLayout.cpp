//
// Created by Albert on 16/03/2023.
//
#include "GridLayout.hpp"

void templater::support::printGrid(
    std::ostream& stream,
    const std::vector<std::string>& table,
    size_t cols,
    std::string_view sep)
{
    // Find the maximum length of string in each column and pad the strings with spaces
    std::vector<size_t> widths(cols);
    for (size_t idx = 0; idx < table.size(); idx++) {
        const size_t col = idx % cols;
        widths[col] = std::max(widths[col], table[idx].length());
    }

    // print out
    for (size_t idx = 0; idx < table.size(); idx++) {
        const size_t col = idx % cols;
        if (col == 0) {
            fmt::print(stream, "\n    ");
        } else {
            fmt::print(stream, "{}", sep);
        }
        fmt::print(stream,
            "{0}{1:<{2}}", table[idx],
            "", (widths[col] - table[idx].length()));
    }
}

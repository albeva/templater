//
// Created by Albert on 04/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater::table {

class Table final {
public:
    NO_COPY_AND_MOVE(Table)
    ~Table() = default;

    void inherit(Table& table);

private:
    using Row = std::unordered_map<std::string, std::string>;
    std::vector<Row> m_rows {};
};

} // namespace templater::table

//
// Created by Albert on 13/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/SourceLoc.hpp"

namespace templater::table {
class Table;
class SymbolTable;

class Symbol final {
public:
    using Value = std::variant<Table*, SymbolTable*>;

    constexpr Symbol(std::string_view name, support::SourceLoc loc, Value value)
        : m_name(name)
        , m_loc(loc)
        , m_value(value)
    {
    }

    [[nodiscard]] constexpr auto getName() const -> const auto& { return m_name; }
    [[nodiscard]] constexpr auto getLoc() const -> const auto& { return m_loc; }
    [[nodiscard]] constexpr auto getValue() const -> const auto& { return m_value; }

private:
    std::string_view m_name;
    support::SourceLoc m_loc;
    Value m_value;
};

} // namespace templater::table

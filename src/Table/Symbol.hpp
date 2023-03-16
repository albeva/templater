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

    Symbol(std::string_view name, support::SourceLoc loc, Value value) noexcept
        : m_name(name)
        , m_loc(loc)
        , m_value(value)
    {
    }

    [[nodiscard]] inline auto getName() const noexcept -> const auto& { return m_name; }
    [[nodiscard]] inline auto getLoc() const noexcept -> const auto& { return m_loc; }
    [[nodiscard]] inline auto getValue() const noexcept -> const auto& { return m_value; }

private:
    std::string_view m_name;
    support::SourceLoc m_loc;
    Value m_value;
};

} // namespace templater::table

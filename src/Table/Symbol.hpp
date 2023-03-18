//
// Created by Albert on 13/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/GlobalContext.hpp"
#include "Support/SourceLoc.hpp"

namespace table {
class Table;
class SymbolTable;

class Symbol final {
public:
    NO_COPY_AND_MOVE(Symbol)

    using SymValue = std::variant<support::GlobalContext::UniquePtr<Table>, support::GlobalContext::UniquePtr<SymbolTable>>;
    Symbol(std::string_view name, support::SourceLoc loc, SymValue value) noexcept;
    ~Symbol();

    [[nodiscard]] inline auto getName() const noexcept -> const auto& { return m_name; }
    [[nodiscard]] inline auto getLoc() const noexcept -> const auto& { return m_loc; }
    [[nodiscard]] inline auto getValue() const noexcept -> const auto& { return m_value; }

private:
    std::string_view m_name;
    support::SourceLoc m_loc;
    SymValue m_value;
};

} // namespace table

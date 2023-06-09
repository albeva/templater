//
// Created by Albert on 04/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Value.hpp"

namespace table {
class SymbolTable;
class Column;

class Table final {
public:
    NO_COPY_AND_MOVE(Table)
    explicit Table(SymbolTable* owner);
    ~Table();

    [[nodiscard]] inline auto getOwner() const noexcept -> SymbolTable* { return m_owner; }

    [[nodiscard]] inline auto getColumns() const noexcept -> const auto& { return m_columns; }
    [[nodiscard]] auto findColumn(std::string_view name) const noexcept -> Column*;
    void addColumn(Column* column);

    void addRow();
    [[nodiscard]] inline auto getRowCount() const noexcept { return m_data.size(); }

    void addValue(size_t row, const Column* column, Value value);
    [[nodiscard]] auto getValue(size_t row, const Column* column) const noexcept -> std::optional<Value>;
    [[nodiscard]] inline auto getRow(size_t row) const noexcept -> const auto& { return m_data[row]; }

private:
    using Row = std::pmr::unordered_map<const Column*, Value>;
    using Data = std::pmr::vector<Row>;

    SymbolTable* m_owner;
    std::pmr::vector<Column*> m_columns;
    Data m_data;
};

} // namespace table

//
// Created by Albert on 04/03/2023.
//
#include "Table.hpp"
#include "Column.hpp"
#include "SymbolTable.hpp"

using table::Table;

Table::Table(SymbolTable* owner)
    : m_owner(owner)
    , m_columns(owner->getPool()->getAllocator())
    , m_data(owner->getPool()->getAllocator())
{
}

Table::~Table() = default;

auto Table::findColumn(std::string_view name) const noexcept -> Column*
{
    auto res = std::ranges::find_if(m_columns, [&](const Column* col) {
        return col->getName() == name;
    });
    return res != m_columns.end() ? *res : nullptr;
}

void Table::addColumn(Column* column)
{
    m_columns.push_back(column);
}

void Table::addRow()
{
    m_data.emplace_back(Row(m_owner->getPool()->getAllocator()));
}

void Table::addValue(size_t row, const Column* column, Value value)
{
    m_data[row].insert({ column, value });
}

auto Table::getValue(size_t row, const Column* column) const noexcept -> std::optional<Value>
{
    auto iter = m_data[row].find(column);
    if (iter != m_data[row].end()) {
        return iter->second;
    }
    return {};
}

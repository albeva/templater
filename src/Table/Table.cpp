//
// Created by Albert on 04/03/2023.
//
#include "Table.hpp"
#include "Column.hpp"
using support::GlobalContext;
using table::Table;

Table::Table(GlobalContext* ctx)
    : m_ctx(ctx)
    , m_columns(ctx->getAllocator())
    , m_data(ctx->getAllocator())
{
    (void)m_ctx;
}

Table::~Table() = default;

auto Table::findColumn(std::string_view name) const noexcept -> Column*
{
    auto res = std::ranges::find_if(m_columns, [&](const support::GlobalContext::UniquePtr<Column>& col) {
        return col->getName() == name;
    });
    return res != m_columns.end() ? res->get() : nullptr;
}

void Table::addColumn(support::GlobalContext::UniquePtr<Column> column)
{
    m_columns.push_back(std::move(column));
}

void Table::addRow()
{
    m_data.emplace_back(Row(m_ctx->getAllocator()));
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

//
// Created by Albert on 04/03/2023.
//
#include "Table.hpp"
#include "Column.hpp"
#include "Support/Context.hpp"
using namespace templater::table;

Table::Table(templater::Context* ctx)
    : m_ctx(ctx)
    , m_columns(ctx->getAllocator())
    , m_data(ctx->getAllocator())
{
    (void)m_ctx;
}

auto Table::findColumn(std::string_view name) const -> Column*
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

auto Table::addRow() -> size_t
{
    m_data.emplace_back(Row { m_ctx->getAllocator() });
    return m_data.size() - 1;
}

auto Table::addValue(size_t row, Value value) -> size_t
{
    m_data[row].emplace_back(value);
    return m_data[row].size() - 1;
}

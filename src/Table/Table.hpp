//
// Created by Albert on 04/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Value.hpp"

namespace templater::support {
class Context;
}

namespace templater::table {
class Column;

class Table final {
public:
    NO_COPY_AND_MOVE(Table)
    explicit Table(support::Context* ctx);
    ~Table() = default;

    [[nodiscard]] inline auto getColumns() const -> const auto& { return m_columns; }
    [[nodiscard]] auto findColumn(std::string_view name) const -> Column*;
    void addColumn(Column* column);

    void addRow();
    [[nodiscard]] auto getRowCount() const { return m_data.size(); }

    void addValue(size_t row, const Column* column, Value value);
    [[nodiscard]] auto getValue(size_t row, const Column* column) -> std::optional<Value>;
    [[nodiscard]] auto getRow(size_t row) const -> const auto& { return m_data[row]; }

private:
    using Row = std::pmr::unordered_map<const Column*, Value>;
    using Data = std::pmr::vector<Row>;

    support::Context* m_ctx;
    std::pmr::vector<Column*> m_columns;
    Data m_data;
};

} // namespace templater::table

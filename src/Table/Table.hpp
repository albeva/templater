//
// Created by Albert on 04/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/GlobalContext.hpp"
#include "Value.hpp"

namespace support {
class GlobalContext;
}

namespace table {
class Column;

class Table final {
public:
    NO_COPY_AND_MOVE(Table)
    explicit Table(support::GlobalContext* ctx);
    ~Table();

    [[nodiscard]] inline auto getColumns() const noexcept -> const auto& { return m_columns; }
    [[nodiscard]] auto findColumn(std::string_view name) const noexcept -> Column*;
    void addColumn(support::GlobalContext::UniquePtr<Column> column);

    void addRow();
    [[nodiscard]] inline auto getRowCount() const noexcept { return m_data.size(); }

    void addValue(size_t row, const Column* column, Value value);
    [[nodiscard]] auto getValue(size_t row, const Column* column) const noexcept -> std::optional<Value>;
    [[nodiscard]] inline auto getRow(size_t row) const noexcept -> const auto& { return m_data[row]; }

private:
    using Row = std::pmr::unordered_map<const Column*, Value>;
    using Data = std::pmr::vector<Row>;

    support::GlobalContext* m_ctx;
    std::pmr::vector<support::GlobalContext::UniquePtr<Column>> m_columns;
    Data m_data;
};

} // namespace table

//
// Created by Albert on 04/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Value.hpp"

namespace templater {
class Context;
}

namespace templater::table {
struct Column;

class Table final {
public:
    NO_COPY_AND_MOVE(Table)
    explicit Table(Context* ctx);
    ~Table() = default;

    [[nodiscard]] auto getColumns() const -> const auto& { return m_columns; }
    [[nodiscard]] auto findColumn(std::string_view name) const -> Column*;
    void addColumn(Column* column);

    [[maybe_unused]] auto addRow() -> size_t;
    [[maybe_unused]] auto addValue(size_t row, Value value) -> size_t;

private:
    using Row = std::pmr::vector<Value>;
    using Data = std::pmr::vector<Row>;

    Context* m_ctx;
    std::pmr::vector<Column*> m_columns;
    Data m_data;
};

} // namespace templater::table

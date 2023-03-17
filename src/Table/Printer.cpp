//
// Created by Albert on 15/03/2023.
//
#include "Printer.hpp"
#include "Column.hpp"
#include "Support/GridLayout.hpp"
#include "Support/Separator.hpp"
#include "Support/Source.hpp"
#include "Symbol.hpp"
#include "SymbolTable.hpp"
#include "Table.hpp"
using support::printGrid;
using support::Separator;
using table::Printer;
using namespace std::literals;

Printer::Printer(const SymbolTable* symbolTable)
    : m_output()
    , m_symbol(nullptr)
{
    for (const auto& symbol : symbolTable->getSymbols()) {
        m_symbol = symbol.get();
        visit(m_symbol->getValue());
    }
}

auto Printer::output() const -> std::string
{
    return m_output.str();
}

void Printer::visit(const support::Context::UniquePtr<SymbolTable>& symbolTable)
{
    fmt::print(m_output,
        "import {} as {}\n",
        symbolTable->getSource()->getName(), m_symbol->getName());
}

void Printer::visit(const support::Context::UniquePtr<Table>& table)
{
    fmt::print(m_output, "table {}", m_symbol->getName());

    // columns
    const auto& columns = table->getColumns();
    fmt::print(m_output, "(");
    Separator sep { " " };
    for (const auto& col : columns) {
        m_output << sep();
        visit(col);
    }
    fmt::print(m_output, ") = [");

    // store all content in a table
    std::vector<std::string> content {};
    content.reserve(table->getRowCount() * columns.size());

    // iterate over rows and columns and collect the data
    for (size_t rowIdx = 0; rowIdx < table->getRowCount(); rowIdx++) {
        const auto& row = table->getRow(rowIdx);
        for (const auto& col : columns) {
            if (auto iter = row.find(col.get()); iter != row.end()) {
                content.emplace_back(toString(iter->second));
            } else {
                content.emplace_back("-");
            }
        }
    }

    // print out
    printGrid(m_output, content, columns.size());
    fmt::print(m_output, "\n]\n");
}

void Printer::visit(const support::Context::UniquePtr<Column>& column)
{
    fmt::print(m_output, "{}", column->getName());
    if (auto value = column->getValue()) {
        fmt::print(m_output, " = {}", toString(value.value()));
    }
}

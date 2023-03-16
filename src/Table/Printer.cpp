//
// Created by Albert on 15/03/2023.
//
#include "Printer.hpp"
#include "Column.hpp"
#include "Support/Separator.hpp"
#include "Support/Source.hpp"
#include "Symbol.hpp"
#include "SymbolTable.hpp"
#include "Table.hpp"
using templater::support::Separator;
using templater::table::Printer;
using namespace std::literals;

Printer::Printer(const SymbolTable* symbolTable)
{
    for (const auto& symbol : symbolTable->getSymbols()) {
        m_symbol = symbol;
        visit(symbol->getValue());
    }
}

auto Printer::output() const -> std::string
{
    return m_output.str();
}

void Printer::visit(const SymbolTable* symbolTable)
{
    fmt::print(m_output,
        "import {} as {}\n",
        symbolTable->getSource()->getName(), m_symbol->getName());
}

void Printer::visit(const Table* table)
{
    fmt::print(m_output, "table {}", m_symbol->getName());

    // columns
    const auto& columns = table->getColumns();
    fmt::print(m_output, "(");
    Separator sep { " " };
    for (const auto* col : columns) {
        fmt::print(m_output, "{}", sep());
        visit(col);
    }
    fmt::print(m_output, ") = [");
    auto holder = std::move(m_output);

    // store all content in a table
    using StringTable = std::vector<std::string>;
    StringTable content {};
    content.reserve(table->getRowCount() * columns.size());

    // iterate over rows and columns and collect the data
    for (size_t index = 0; index < table->getRowCount(); index++) {
        const auto& row = table->getRow(index);
        for (const auto* col : columns) {
            if (auto iter = row.find(col); iter != row.end()) {
                m_output = std::stringstream {};
                visit(iter->second);
                content.emplace_back(m_output.str());
            } else {
                content.emplace_back("-");
            }
        }
    }

    // measure each column
    std::vector<size_t> widths(columns.size());
    for (size_t idx = 0; idx < content.size(); idx++) {
        const size_t col = idx % columns.size();
        widths[col] = std::max(widths[col], content[idx].length());
    }

    // print out
    m_output = std::move(holder);
    for (size_t idx = 0; idx < content.size(); idx++) {
        const size_t col = idx % columns.size();
        if (col == 0) {
            fmt::print(m_output, "\n   ");
        }
        fmt::print(m_output,
            " {0}{1:<{2}}", content[idx],
            "", (widths[col] - content[idx].length()));
    }
    fmt::print(m_output, "\n]\n");
}

void Printer::visit(const Column* column)
{
    fmt::print(m_output, "{}", column->getName());
    if (auto value = column->getValue()) {
        fmt::print(m_output, " = ");
        visit(value.value());
    }
}

void Printer::visit(const PipeLiteral& /*pipe*/)
{
    fmt::print(m_output, "|");
}

void Printer::visit(const Identifier& token)
{
    fmt::print(m_output, "{}", token.getValue());
}

void Printer::visit(const NumberLiteral& token)
{
    fmt::print(m_output, "{}", token.getValue());
}

void Printer::visit(const StringLiteral& token)
{
    m_output << std::quoted(token.getValue());
}
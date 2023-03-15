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
    m_output << " (";
    Separator sep { ", " };
    for (const auto* col : columns) {
        m_output << sep();
        visit(col);
    }
    m_output << ")";

    // rows
    m_output << " = [\n";
    for (size_t index = 0; index < table->getRowCount(); index++) {
        m_output << "    ";
        const auto& row = table->getRow(index);
        Separator sep2 { " " };
        for (const auto* col : columns) {
            m_output << sep2();
            if (auto iter = row.find(col); iter != row.end()) {
                visit(iter->second);
            } else {
                m_output << '-';
            }
        }
        m_output << "\n";
    }
    m_output << "]\n\n";
}

void Printer::visit(const Column* column)
{
    m_output << column->getName();
    if (auto value = column->getValue()) {
        m_output << " = ";
        visit(value.value());
    }
}

void Printer::visit(const Identifier& token)
{
    m_output << token.getValue();
}

void Printer::visit(const NumberLiteral& token)
{
    m_output << token.getValue();
}

void Printer::visit(const StringLiteral& token)
{
    m_output << std::quoted(token.getValue());
}
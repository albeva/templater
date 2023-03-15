//
// Created by Albert on 15/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/VisitorMixin.hpp"
#include "Value.hpp"
namespace templater::table {
class SymbolTable;
class Symbol;
class Table;
class Column;

class Printer final {
public:
    NO_COPY_AND_MOVE(Printer)
    explicit Printer(const SymbolTable* symbolTable);
    ~Printer() = default;

    VISITOR_MIXIN

    [[nodiscard]] auto output() const -> std::string;

private:
    void visit(const SymbolTable* symbolTable);
    void visit(const Table* table);
    void visit(const Column* column);

    void visit(const Identifier& token);
    void visit(const NumberLiteral& token);
    void visit(const StringLiteral& token);

    std::stringstream m_output {};
    Symbol* m_symbol = nullptr;
};

auto inline operator<<(std::ostream& os, const Printer& printer) -> std::ostream&
{
    os << printer.output();
    return os;
}

} // namespace templater::table

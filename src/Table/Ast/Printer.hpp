//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
#include "Support/VisitorMixin.hpp"
#include "Table/Value.hpp"
namespace templater::table::ast {

class Printer final {
public:
    NO_COPY_AND_MOVE(Printer)
    Printer() = default;
    explicit Printer(const Content* node) { visit(node); }
    ~Printer() = default;

    VISITOR_MIXIN

    [[nodiscard]] auto output() const -> std::string;

private:
    void visit(const Content* node);
    void visit(const Import* node);
    void visit(const Table* node);
    void visit(const TableColumn* node);
    void visit(const TableInherit* node);
    void visit(const TableBody* node);
    void visit(const TableRow* node);
    void visit(const StructBody* node);
    void visit(const UnaryExpression* node);
    void visit(const BinaryExpression* node);
    void visit(const Member* node);

    void visit(const Identifier& node);
    void visit(const StringLiteral& node);
    void visit(const NumberLiteral& node);

    std::size_t m_indent = 0;
    std::stringstream m_output {};
    [[nodiscard]] auto spaces() const { return std::string(m_indent * 4, ' '); }
};

auto inline operator<<(std::ostream& os, const Printer& printer) -> std::ostream&
{
    os << printer.output();
    return os;
}

} // namespace templater::table::ast

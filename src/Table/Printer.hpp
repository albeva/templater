//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "Visitor.hpp"

namespace templater::table::ast {

class Printer final : public Visitor<Printer> {
public:
    using Visitor::visit;

    Printer() = default;

    explicit Printer(StatementList& node)
    {
        visit(node);
    }

    void visit(StatementList& node);
    void visit(Import& node);
    void visit(Table& node);
    void visit(TableColumn& node);
    void visit(TableInherit& node);
    void visit(TableBody& node);
    void visit(TableRow& node);
    void visit(TableValue& node);
    void visit(StructBody& node);
    void visit(UnaryExpression& node);
    void visit(BinaryExpression& node);
    void visit(Literal& node);
    void visit(Member& node);

    [[nodiscard]] auto output() const -> std::string;

private:
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

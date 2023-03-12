//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "Visitor.hpp"

namespace templater::table::ast {

class Printer final : public Visitor {
public:
    using Visitor::visit;

    void visit(StatementList& node) override;
    void visit(Import& node) override;
    void visit(Table& node) override;
    void visit(TableColumn& node) override;
    void visit(TableInherit& node) override;
    void visit(TableBody& node) override;
    void visit(TableRow& node) override;
    void visit(TableValue& node) override;
    void visit(StructBody& node) override;
    void visit(UnaryExpression& node) override;
    void visit(BinaryExpression& node) override;
    void visit(Literal& node) override;
    void visit(Member& node) override;

private:
    std::size_t m_indent = 0;
    [[nodiscard]] auto spaces() const { return std::string(m_indent * 4, ' '); }
};

} // namespace templater::table::ast

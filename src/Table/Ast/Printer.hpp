//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
#include "Support/VisitorMixin.hpp"
#include "Table/Value.hpp"
namespace table::ast {

class Printer final {
public:
    NO_COPY_AND_MOVE(Printer)
    Printer() = default;
    explicit Printer(const Node<Content>& node) { visit(node); }
    ~Printer() = default;

    VISITOR_MIXIN

    [[nodiscard]] auto output() const { return m_output.str(); }

private:
    void visit(const Node<Content>& node);
    void visit(const Node<Import>& node);
    void visit(const Node<Table>& node);
    void visit(const Node<TableColumn>& node);
    void visit(const Node<TableInherit>& node);
    void visit(const Node<TableBody>& node);
    static void visit(std::vector<std::string>& dst, const Node<TableRow>& node);

    static auto visit(const Value& value) -> std::string;
    static auto visit(const Node<Member>& node) -> std::string;
    auto visit(const Node<UnaryExpression>& node) -> std::string;
    auto visit(const Node<BinaryExpression>& node) -> std::string;

    std::stringstream m_output {};
};

auto inline operator<<(std::ostream& os, const Printer& printer) -> std::ostream&
{
    os << printer.output();
    return os;
}

} // namespace table::ast

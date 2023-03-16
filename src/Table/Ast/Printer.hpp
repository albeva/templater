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

    [[nodiscard]] auto output() const { return m_output.str(); }

private:
    void visit(const Content* node);
    void visit(const Import* node);
    void visit(const Table* node);
    void visit(const TableColumn* node);
    void visit(const TableInherit* node);
    void visit(const TableBody* node);
    static void visit(std::vector<std::string>& dst, const TableRow* node);

    static auto visit(const Value& value) -> std::string;
    static auto visit(const Member* node) -> std::string;
    auto visit(const UnaryExpression* node) -> std::string;
    auto visit(const BinaryExpression* node) -> std::string;

    std::stringstream m_output {};
};

auto inline operator<<(std::ostream& os, const Printer& printer) -> std::ostream&
{
    os << printer.output();
    return os;
}

} // namespace templater::table::ast

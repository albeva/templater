//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Visitor.hpp"

namespace templater::table::ast {

class Printer final : public Visitor<Printer> {
public:
    NO_COPY_AND_MOVE(Printer)
    using Visitor::visit;
    Printer() = default;
    explicit Printer(Content& node)
    {
        visit(node);
    }
    ~Printer() = default;

    void visit(const Content& node);
    void visit(const Import& node);
    void visit(const Table& node);
    void visit(const TableColumn& node);
    void visit(const TableInherit& node);
    void visit(const TableBody& node);
    void visit(const TableRow& node);
    void visit(const TableValue& node);
    void visit(const StructBody& node);
    void visit(const UnaryExpression& node);
    void visit(const BinaryExpression& node);
    void visit(const Literal& node);
    void visit(const Member& node);

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

//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Visitor.hpp"

namespace templater {
class Context;
}

namespace templater::table {
class SymbolTable;

class Generator final : public ast::Visitor<Generator> {
public:
    NO_COPY_AND_MOVE(Generator)
    using Visitor::visit;
    Generator(Context* ctx, ast::Content& node);
    ~Generator() = default;

    void visit(const ast::Content& node);
    void visit(const ast::Import& node);
    void visit(const ast::Table& node);
    void visit(const ast::TableColumn& node);
    void visit(const ast::TableInherit& node);
    void visit(const ast::TableBody& node);
    void visit(const ast::TableRow& node);
    void visit(const ast::TableValue& node);
    void visit(const ast::StructBody& node);
    void visit(const ast::UnaryExpression& node);
    void visit(const ast::BinaryExpression& node);
    void visit(const ast::Literal& node);
    void visit(const ast::Member& node);

private:
    SymbolTable* m_symbolTable;
    Context* m_ctx;
};

} // namespace templater::table

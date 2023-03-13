//
// Created by Albert on 12/03/2023.
//
#include "Generator.hpp"
#include "Support/Context.hpp"
#include "Symbol.hpp"
#include "SymbolTable.hpp"
using namespace templater::table;

Generator::Generator(templater::Context* ctx, ast::Content& node)
    : m_ctx(ctx)
    , m_symbolTable(ctx->create<SymbolTable>(ctx))
{
    visit(node);
}

void Generator::visit(const ast::Content& node)
{
    for (const auto* stmt : node.getStatements()) {
        visit(*stmt);
    }
}

void Generator::visit(const ast::Import& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::Table& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::TableColumn& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::TableInherit& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::TableBody& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::TableRow& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::TableValue& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::StructBody& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::UnaryExpression& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::BinaryExpression& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::Literal& node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::Member& node)
{
    (void)this;
    (void)node;
}

//
// Created by Albert on 12/03/2023.
//
#include "Generator.hpp"
#include "Support/Context.hpp"
#include "Symbol.hpp"
#include "SymbolTable.hpp"
using namespace templater::table;
using namespace templater::table::ast;

Generator::Generator(templater::Context* ctx, const ast::Content* node)
    : m_ctx(ctx)
    , m_symbolTable(ctx->create<SymbolTable>(ctx))
{
    (void)m_ctx;
    visit(node);
}

void Generator::operator()(const ast::Content* node)
{
    for (const auto& stmt : node->getStatements()) {
        visit(stmt);
    }
}

void Generator::operator()(const ast::Import* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::Table* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::TableColumn* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::TableInherit* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::TableBody* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::TableRow* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const TableValue& node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::StructBody* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::UnaryExpression* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::BinaryExpression* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::Literal* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::Member* node)
{
    (void)this;
    (void)node;
}

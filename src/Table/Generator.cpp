//
// Created by Albert on 12/03/2023.
//
#include "Generator.hpp"
#include "Support/Context.hpp"
#include "Support/Diagnostics.hpp"
#include "Symbol.hpp"
#include "SymbolTable.hpp"
#include "Table.hpp"
using namespace templater::table;
using namespace templater::table::ast;

Generator::Generator(templater::Context* ctx, Diagnostics* diag, Source* source, const ast::Content* node)
    : m_ctx(ctx)
    , m_diag(diag)
    , m_source(source)
    , m_symbolTable(ctx->create<SymbolTable>(ctx))
{
    (void)m_ctx;
    (void)m_diag;
    (void)m_source;
    visit(node);
}

void Generator::operator()(const ast::Content* node)
{
    visitEach(node->getStatements());
}

void Generator::operator()(const ast::Import* node)
{
    (void)this;
    (void)node;
}

void Generator::operator()(const ast::Table* node)
{
    const auto& id = node->getIdentifier();
    const auto* existing = m_symbolTable->find(id.getValue());
    if (existing != nullptr) {
        m_diag->error(m_source, id.getLoc(), fmt::format("redefinition of '{}'", id.getValue()));
        m_diag->notice(m_source, existing->getLoc(), "note: previous definition is here");
        throw GeneratorException("");
    }

    m_table = m_ctx->create<Table>();
    visitEach(node->getColumns());
    visitEach(node->getContent());
    auto* symbol = m_ctx->create<Symbol>(id.getValue(), id.getLoc(), m_table);
    m_symbolTable->insert(symbol);
    m_table = nullptr;
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

//
// Created by Albert on 12/03/2023.
//
#include "Generator.hpp"
#include "Support/Context.hpp"
#include "Support/Diagnostics.hpp"
#include "Table/Column.hpp"
#include "Table/Symbol.hpp"
#include "Table/SymbolTable.hpp"
#include "Table/Table.hpp"
#include "Table/Value.hpp"
#include "TableValue.hpp"
using namespace templater::table::ast;
using namespace templater::table::gen;

Generator::Generator(templater::Context* ctx, Diagnostics* diag, Source* source, const ast::Content* node)
    : m_ctx(ctx)
    , m_diag(diag)
    , m_source(source)
    , m_symbolTable(ctx->create<SymbolTable>(ctx))
{
    visit(node);
}

void Generator::visit(const ast::Content* node)
{
    visitEach(node->getStatements());
}

void Generator::visit(const ast::Import* node)
{
    (void)this;
    (void)node;
}

void Generator::visit(const ast::Table* node)
{
    const auto& id = node->getIdentifier();
    const auto* existing = m_symbolTable->find(id.getValue());
    if (existing != nullptr) {
        redefinition(id, existing->getLoc());
    }

    m_table = m_ctx->create<Table>(m_ctx);
    visitEach(node->getColumns());
    visitEach(node->getContent());
    auto* symbol = m_ctx->create<Symbol>(id.getValue(), id.getLoc(), m_table);
    m_symbolTable->insert(symbol);
    m_table = nullptr;
}

void Generator::visit(const ast::TableColumn* node)
{
    const auto& id = node->getIdentifier();
    const auto* existing = m_table->findColumn(node->getIdentifier().getValue());
    if (existing != nullptr) {
        redefinition(id, existing->getLoc());
    }

    std::optional<Value> value {};
    const auto& nodeVal = node->getValue();
    if (nodeVal.has_value()) {
        value = std::visit(TableValue(), nodeVal.value());
    }

    auto* column = m_ctx->create<Column>(id.getValue(), id.getLoc(), value);
    m_table->addColumn(column);
}

void Generator::visit(const ast::TableInherit* /*node*/)
{
    (void)this;
    throw GeneratorException("Inheritance not implemented");
}

void Generator::visit(const ast::TableBody* node)
{
    visitEach(node->getRows());
}

void Generator::visit(const ast::TableRow* node)
{
    m_rowIndex = m_table->addRow();
    for (const auto& val : node->getValues()) {
        auto value = std::visit(TableValue(), val);
        m_colIndex = m_table->addValue(m_rowIndex, value);
    }
}

void Generator::visit(const ast::Member* node)
{
    (void)this;
    (void)node;
}

void Generator::redefinition(const parser::Token& id, SourceLoc existing) const
{
    m_diag->error(m_source, id.getLoc(), fmt::format("redefinition of '{}'", id.getValue()));
    m_diag->notice(m_source, existing, "previous definition is here");
    throw GeneratorException("");
}

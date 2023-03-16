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
using namespace std::literals;
using templater::support::Context;
using templater::support::Diagnostics;
using templater::support::Source;
using templater::table::gen::Generator;

Generator::Generator(Context* ctx, Diagnostics* diag, Source* source, const ast::Content* node)
    : m_ctx(ctx)
    , m_diag(diag)
    , m_source(source)
    , m_symbolTable(ctx->create<SymbolTable>(ctx, source))
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

    auto* column = m_ctx->create<Column>(id.getValue(), id.getLoc(), node->getValue());
    m_table->addColumn(column);
}

void Generator::visit(const ast::TableInherit* /*node*/)
{
    (void)this;
    throw GeneratorException("Inheritance not implemented");
}

void Generator::visit(const ast::TableBody* node)
{
    m_rowIndex = 0;
    visitEach(node->getRows());
}

void Generator::visit(const ast::TableRow* node)
{
    m_table->addRow();
    const auto& values = node->getValues();

    if (values.size() != m_table->getColumns().size()) {
        throw GeneratorException("Row value count mismatches declared column count.");
    }

    for (size_t col = 0; col < values.size(); col++) {
        const Column* column = m_table->getColumns().at(col);
        std::visit(
            support::Visitor {
                [](const std::monostate&) {
                    // No op?
                },
                [&](const ast::PipeLiteral&) {
                    if (m_table->getRowCount() < 2) {
                        throw GeneratorException("'|' can only be used under another value.");
                    }
                    if (auto previous = m_table->getValue(m_table->getRowCount() - 2, column)) {
                        m_table->addValue(m_rowIndex, column, previous.value());
                    } else {
                        throw GeneratorException("'|' No previous value");
                    }
                },
                [&](const Value& value) {
                    m_table->addValue(m_rowIndex, column, value);
                } },
            values[col]);
    }
    m_rowIndex++;
}

void Generator::visit(const ast::Member* node)
{
    (void)this;
    (void)node;
}

void Generator::redefinition(const Identifier& id, SourceLoc existing) const
{
    m_diag->error(m_source, id.getLoc(), fmt::format("redefinition of '{}'", id.getValue()));
    m_diag->notice(m_source, existing, "previous definition is here");
    throw GeneratorException("");
}

//
// Created by Albert on 12/03/2023.
//
#include "Generator.hpp"
#include "Support/Diagnostics.hpp"
#include "Table/Ast/Context.hpp"
#include "Table/Column.hpp"
#include "Table/Symbol.hpp"
#include "Table/SymbolTable.hpp"
#include "Table/Table.hpp"
#include "Table/Value.hpp"
using namespace std::literals;
using support::Diagnostics;
using table::gen::Generator;

Generator::Generator(Diagnostics* diag)
    : m_diag(diag)
    , m_source(nullptr)
    , m_symbolTable(nullptr)
    , m_table(nullptr)
    , m_rowIndex(0)
{
}

Generator::~Generator() = default;

auto Generator::visit(const ast::Context* ast) -> std::unique_ptr<SymbolTable>
{
    m_source = ast->getSource();
    m_symbolTable = std::make_unique<SymbolTable>(m_source);
    visitEach(ast->getRoot()->getStatements());
    m_source = nullptr;
    return std::move(m_symbolTable);
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

    m_table = m_symbolTable->getPool()->create<Table>(m_symbolTable.get());
    visitEach(node->getColumns());
    visitEach(node->getContent());
    auto* symbol = m_symbolTable->getPool()->create<Symbol>(id.getValue(), id.getLoc(), m_table);
    m_symbolTable->insert(symbol);
}

void Generator::visit(const ast::TableColumn* node)
{
    const auto& id = node->getIdentifier();
    const auto* existing = m_table->findColumn(node->getIdentifier().getValue());
    if (existing != nullptr) {
        redefinition(id, existing->getLoc());
    }

    auto* column = m_symbolTable->getPool()->create<Column>(id.getValue(), id.getLoc(), node->getValue());
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
        const auto& column = m_table->getColumns().at(col);
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
    throw support::EmptyException();
}

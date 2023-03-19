//
// Created by Albert on 19/03/2023.
//
#include "Templater.hpp"
#include "Support/GlobalContext.hpp"
#include "Support/Source.hpp"
using tpl::Templater;

Templater::Templater(support::GlobalContext* ctx, support::Source* source, table::SymbolTable* symbolTable)
    : m_ctx(ctx)
    , m_source(source)
    , m_symbolTable(symbolTable)
{
    (void)m_ctx;
    (void)m_source;
    (void)m_symbolTable;
}

tpl::Templater::~Templater() = default;

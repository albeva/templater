//
// Created by Albert on 13/03/2023.
//
#include "SymbolTable.hpp"
#include "Support/Context.hpp"
using support::Context;
using support::Source;
using table::SymbolTable;

SymbolTable::SymbolTable(Context* ctx, Source* source)
    : m_symbols(ctx->getAllocator())
    , m_source(source)
{
}

auto SymbolTable::find(std::string_view name) const noexcept -> Symbol*
{
    auto iter = m_symbols.find(name);
    if (iter != m_symbols.end()) {
        return *iter;
    }
    return nullptr;
}

auto SymbolTable::contains(std::string_view name) const noexcept -> bool
{
    return m_symbols.contains(name);
}

void SymbolTable::insert(Symbol* symbol)
{
    m_symbols.insert(symbol);
}

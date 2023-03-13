//
// Created by Albert on 13/03/2023.
//
#include "SymbolTable.hpp"
#include "Support/Context.hpp"
using namespace templater::table;

SymbolTable::SymbolTable(Context* ctx)
    : m_symbols(ctx->getAllocator())
{
}

auto SymbolTable::find(std::string_view name) const -> Symbol*
{
    auto iter = m_symbols.find(name);
    if (iter != m_symbols.end()) {
        return *iter;
    }
    return nullptr;
}

auto SymbolTable::contains(std::string_view name) const -> bool
{
    return m_symbols.contains(name);
}

void SymbolTable::insert(Symbol* symbol)
{
    m_symbols.insert(symbol);
}
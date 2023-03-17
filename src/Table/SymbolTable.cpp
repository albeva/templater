//
// Created by Albert on 13/03/2023.
//
#include "SymbolTable.hpp"
#include "Support/Context.hpp"
#include "Symbol.hpp"
#include "Table.hpp"
using support::Context;
using support::Source;
using table::SymbolTable;

SymbolTable::SymbolTable(Context* ctx, Source* source)
    : m_symbols(ctx->getAllocator())
    , m_source(source)
{
}

SymbolTable::~SymbolTable() = default;

auto SymbolTable::find(std::string_view name) const noexcept -> Symbol*
{
    auto iter = m_symbols.find(name);
    if (iter != m_symbols.end()) {
        return iter->get();
    }
    return nullptr;
}

auto SymbolTable::contains(std::string_view name) const noexcept -> bool
{
    return m_symbols.contains(name);
}

void SymbolTable::insert(SymbolPtr symbol)
{
    m_symbols.insert(std::move(symbol));
}

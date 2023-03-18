//
// Created by Albert on 13/03/2023.
//
#include "SymbolTable.hpp"
#include "Table.hpp"
using support::Source;
using table::SymbolTable;

SymbolTable::SymbolTable(Source* source)
    : m_source(source)
    , m_pa(&m_mbr)
    , m_symbols(m_pa)
{
}

SymbolTable::~SymbolTable() = default;

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
    m_symbols.insert(std::move(symbol));
}

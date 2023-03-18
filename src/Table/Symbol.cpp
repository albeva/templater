//
// Created by Albert on 17/03/2023.
//
#include "Symbol.hpp"
#include "SymbolTable.hpp"
#include "Table.hpp"
using table::Symbol;

Symbol::Symbol(std::string_view name, support::SourceLoc loc, SymValue value) noexcept
    : m_name(name)
    , m_loc(loc)
    , m_value(std::move(value))
{
}

Symbol::~Symbol() = default;

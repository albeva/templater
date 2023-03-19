//
// Created by Albert on 19/03/2023.
//

#include "Templater.hpp"
using tpl::Templater;

Templater::Templater(support::Source* source, table::SymbolTable* symbolTable)
    : m_source(source)
    , m_symbolTable(symbolTable)
    , m_output()
{
}

tpl::Templater::~Templater() = default;

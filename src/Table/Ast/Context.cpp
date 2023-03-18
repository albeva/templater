//
// Created by Albert on 18/03/2023.
//
#include "Context.hpp"
using table::ast::Context;

Context::Context(support::Source* source)
    : m_mbr()
    , m_pa(&m_mbr)
    , m_root(nullptr)
    , m_source(source)
{
}

Context::~Context() = default;

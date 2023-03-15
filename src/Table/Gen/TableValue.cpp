//
// Created by Albert on 14/03/2023.
//
#include "TableValue.hpp"
#include "Generator.hpp"
#include "Table/Ast/Ast.hpp"
#include "Table/Parse/Token.hpp"
using templater::table::gen::TableValue;

auto TableValue::operator()(const Identifier& node) const -> Value
{
    return node;
}

auto TableValue::operator()(const StringLiteral& node) const -> Value
{
    return node;
}

auto TableValue::operator()(const NumberLiteral& node) const -> Value
{
    return node;
}

auto TableValue::operator()(const ast::StructBody* node) const -> Value
{
    (void)node;
    throw GeneratorException("struct values not supported yet");
}

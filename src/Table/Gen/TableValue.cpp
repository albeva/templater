//
// Created by Albert on 14/03/2023.
//
#include "TableValue.hpp"
#include "Table/Ast.hpp"
#include "Table/Generator.hpp"
#include "Table/Token.hpp"
using namespace templater::table::gen;

auto TableValue::operator()(const Token& node) const -> Value
{
    return node;
}

auto TableValue::operator()(const ast::StructBody* node) const -> Value
{
    (void)node;
    throw GeneratorException("struct values not supported yet");
}

//
// Created by Albert on 05/03/2023.
//
#include "Visitor.hpp"
#include "Ast.hpp"
using namespace templater::table::ast;

#define VISIT(KIND)                                          \
    if (node.kind == Kind::KIND) {                           \
        return visit(static_cast<KIND&>(node)); /* NOLINT */ \
    }

void Visitor::visit(Statement& node)
{
    VISIT(Import)
    VISIT(Table)
}

void Visitor::visit(Expression& node)
{
    VISIT(UnaryExpression)
    VISIT(BinaryExpression)
    VISIT(TableValue)
}

void Visitor::visit(TableContent& node)
{
    VISIT(TableInherit)
    VISIT(TableBody)
}

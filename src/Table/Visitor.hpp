//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
namespace templater::table::ast {

#define VISIT(KIND)                                                                     \
    if (node.kind == Kind::KIND) {                                                      \
        return static_cast<Super*>(this)->visit(static_cast<KIND&>(node)); /* NOLINT */ \
    }

template <typename Super>
struct Visitor {

    void visit(Statement& node)
    {
        VISIT(Import)
        VISIT(Table)
    }

    void visit(Expression& node)
    {
        VISIT(UnaryExpression)
        VISIT(BinaryExpression)
        VISIT(TableValue)
    }

    void visit(TableContent& node)
    {
        VISIT(TableInherit)
        VISIT(TableBody)
    }
};

#undef VISIT

} // namespace templater::table::ast

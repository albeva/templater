//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
namespace templater::table::ast {

#define VISIT(KIND)                                                        \
    if (node.getKind() == Kind::KIND) {                                    \
        return static_cast<Super*>(this)->visit(static_cast<KIND&>(node)); \
    }

// We know static casts here are safe.
// NOLINTBEGIN cppcoreguidelines-pro-type-static-cast-downcast

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

// NOLINTEND

#undef VISIT

} // namespace templater::table::ast

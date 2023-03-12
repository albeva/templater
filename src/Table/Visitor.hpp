//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
namespace templater::table::ast {

struct Expression;
struct Statement;
struct StatementList;
struct Import;
struct Table;
struct TableColumn;
struct TableContent;
struct TableInherit;
struct TableBody;
struct TableRow;
struct TableValue;
struct StructBody;
struct UnaryExpression;
struct BinaryExpression;
struct Literal;
struct Member;

struct Visitor {
    NO_COPY_AND_MOVE(Visitor)
    Visitor() = default;
    virtual ~Visitor() = default;

    virtual void visit(StatementList& node) = 0;
    virtual void visit(Import& node) = 0;
    virtual void visit(Table& node) = 0;
    virtual void visit(TableColumn& node) = 0;
    virtual void visit(TableInherit& node) = 0;
    virtual void visit(TableBody& node) = 0;
    virtual void visit(TableRow& node) = 0;
    virtual void visit(TableValue& node) = 0;
    virtual void visit(StructBody& node) = 0;
    virtual void visit(UnaryExpression& node) = 0;
    virtual void visit(BinaryExpression& node) = 0;
    virtual void visit(Literal& node) = 0;
    virtual void visit(Member& node) = 0;

    void visit(Statement& node);
    void visit(Expression& node);
    void visit(TableContent& node);
};
} // namespace templater::table::ast

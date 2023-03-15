//
// Created by Albert on 05/03/2023.
//
#include "Ast.hpp"
using namespace templater::table::ast;
using templater::support::SourceLoc;

Content::Content(SourceLoc loc, List<Statement> statements)
    : Root(loc)
    , m_statements(std::move(statements))
{
}

Import::Import(SourceLoc loc, Identifier identifier, StringLiteral file)
    : Root(loc)
    , m_identifier(identifier)
    , m_file(file)
{
}

Table::Table(SourceLoc loc, Identifier identifier, List<TableColumn*> columns, List<TableContent> content)
    : Root(loc)
    , m_identifier(identifier)
    , m_columns(std::move(columns))
    , m_content(std::move(content))
{
}

TableColumn::TableColumn(SourceLoc loc, Identifier identifier, std::optional<TableValue> value)
    : Root(loc)
    , m_identifier(identifier)
    , m_value(value)
{
}

TableInherit::TableInherit(SourceLoc loc, Member* member, std::optional<Expression> expression)
    : Root(loc)
    , m_member(member)
    , m_expression(expression)
{
}

TableBody::TableBody(SourceLoc loc, List<TableRow*> rows)
    : Root(loc)
    , m_rows(std::move(rows))
{
}

TableRow::TableRow(SourceLoc loc, List<TableValue> values)
    : Root(loc)
    , m_values(std::move(values))
{
}

UnaryExpression::UnaryExpression(SourceLoc loc, Operation op, Expression rhs)
    : Root(loc)
    , m_op(op)
    , m_rhs(rhs)
{
}

BinaryExpression::BinaryExpression(SourceLoc loc, Operation op, Expression lhs, Expression rhs)
    : Root(loc)
    , m_op(op)
    , m_lhs(lhs)
    , m_rhs(rhs)
{
}

Member::Member(SourceLoc loc, List<Identifier> identifiers)
    : Root(loc)
    , m_identifiers(std::move(identifiers))
{
}

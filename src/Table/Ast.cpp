//
// Created by Albert on 05/03/2023.
//
#include "Ast.hpp"

#include <utility>
using namespace templater::table::ast;

StatementList::StatementList(SourceLoc loc, List<Statement> statements)
    : Root { Kind::StatementList, loc }
    , statements { std::move(statements) }
{
}

Import::Import(SourceLoc loc, std::string_view file, std::string_view identifier)
    : Statement { Kind::Import, loc }
    , file { file }
    , identifier { identifier }
{
}

Table::Table(SourceLoc loc, std::string_view identifier, List<TableColumn> columns, List<TableContent> content)
    : Statement { Kind::Table, loc }
    , identifier { identifier }
    , columns { std::move(columns) }
    , content { std::move(content) }
{
}

TableColumn::TableColumn(SourceLoc loc, std::string_view identifier, TableValue* value)
    : Root { Kind::TableColumn, loc }
    , identifier { identifier }
    , value { value }
{
}

TableInherit::TableInherit(SourceLoc loc, Member* member, Expression* expression)
    : TableContent { Kind::TableInherit, loc }
    , member { member }
    , expression { expression }
{
}

TableBody::TableBody(SourceLoc loc, List<TableRow> rows)
    : TableContent { Kind::TableBody, loc }
    , rows { std::move(rows) }
{
}

TableRow::TableRow(SourceLoc loc, List<TableValue> values)
    : Root { Kind::TableRow, loc }
    , values { std::move(values) }
{
}

TableValue::TableValue(SourceLoc loc, Literal* literal)
    : Expression { Kind::TableValue, loc }
    , value { literal }
{
}

TableValue::TableValue(SourceLoc loc, StructBody* structBody)
    : Expression { Kind::TableValue, loc }
    , value { structBody }
{
}

UnaryExpression::UnaryExpression(SourceLoc loc, TokenKind type, Expression* rhs)
    : Expression { Kind::UnaryExpression, loc }
    , type { type }
    , rhs { rhs }
{
}

BinaryExpression::BinaryExpression(SourceLoc loc, TokenKind type, Expression* lhs, Expression* rhs)
    : Expression { Kind::BinaryExpression, loc }
    , type { type }
    , lhs { lhs }
    , rhs { rhs }
{
}

Literal::Literal(SourceLoc loc, TokenKind type, std::string_view value)
    : Root { Kind::Literal, loc }
    , type { type }
    , value { value }
{
}

Member::Member(SourceLoc loc, std::pmr::vector<std::string_view> identifiers)
    : Root { Kind::Member, loc }
    , identifiers { std::move(identifiers) }
{
}

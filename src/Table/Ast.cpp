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

TableColumn::TableColumn(SourceLoc loc, std::string_view identifier, Node<TableValue> value)
    : Root { Kind::TableColumn, loc }
    , identifier { identifier }
    , value { std::move(value) }
{
}

TableInherit::TableInherit(SourceLoc loc, Node<Member> member, Node<Expression> expression)
    : TableContent { Kind::TableInherit, loc }
    , member { std::move(member) }
    , expression { std::move(expression) }
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

TableValue::TableValue(SourceLoc loc, Node<Literal> literal)
    : Expression { Kind::TableValue, loc }
    , value { std::move(literal) }
{
}

TableValue::TableValue(SourceLoc loc, Node<StructBody> structBody)
    : Expression { Kind::TableValue, loc }
    , value { std::move(structBody) }
{
}

UnaryExpression::UnaryExpression(SourceLoc loc, TokenKind type, Node<Expression> rhs)
    : Expression { Kind::UnaryExpression, loc }
    , type { type }
    , rhs { std::move(rhs) }
{
}

BinaryExpression::BinaryExpression(SourceLoc loc, TokenKind type, Node<Expression> lhs, Node<Expression> rhs)
    : Expression { Kind::BinaryExpression, loc }
    , type { type }
    , lhs { std::move(lhs) }
    , rhs { std::move(rhs) }
{
}

Literal::Literal(SourceLoc loc, TokenKind type, std::string_view value)
    : Root { Kind::Literal, loc }
    , type { type }
    , value { value }
{
}

Member::Member(SourceLoc loc, std::vector<std::string_view> identifiers)
    : Root { Kind::Member, loc }
    , identifiers { std::move(identifiers) }
{
}

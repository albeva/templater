//
// Created by Albert on 05/03/2023.
//
#include "Ast.hpp"

#include <utility>
using namespace templater::table::ast;

StatementList::StatementList(List<Statement> statements)
    : Root { Kind::StatementList }
    , statements { std::move(statements) }
{
}

Import::Import(std::string_view file, std::string_view identifier)
    : Statement { Kind::Import }
    , file { file }
    , identifier { std::move(identifier) }
{
}

Table::Table(std::string_view identifier, List<TableColumn> columns, List<TableContent> content)
    : Statement { Kind::Table }
    , identifier { identifier }
    , columns { std::move(columns) }
    , content { std::move(content) }
{
}

TableColumn::TableColumn(std::string_view identifier, Node<TableValue> value)
    : Root { Kind::TableColumn }
    , identifier { identifier }
    , value { std::move(value) }
{
}

TableInherit::TableInherit(Node<Member> member, Node<Expression> expression)
    : TableContent { Kind::TableInherit }
    , member { std::move(member) }
    , expression { std::move(expression) }
{
}

TableBody::TableBody(List<TableRow> rows)
    : TableContent { Kind::TableBody }
    , rows { std::move(rows) }
{
}

TableRow::TableRow(List<TableValue> values)
    : Root { Kind::TableRow }
    , values { std::move(values) }
{
}

TableValue::TableValue(Node<Literal> literal)
    : Expression { Kind::TableValue }
    , value { std::move(literal) }
{
}

TableValue::TableValue(Node<StructBody> structBody)
    : Expression { Kind::TableValue }
    , value { std::move(structBody) }
{
}

UnaryExpression::UnaryExpression(TokenKind type, Node<Expression> rhs)
    : Expression { Kind::UnaryExpression }
    , type { type }
    , rhs { std::move(rhs) }
{
}

BinaryExpression::BinaryExpression(TokenKind type, Node<Expression> lhs, Node<Expression> rhs)
    : Expression { Kind::BinaryExpression }
    , type { type }
    , lhs { std::move(lhs) }
    , rhs { std::move(rhs) }
{
}

Literal::Literal(TokenKind type, std::string_view value)
    : Root { Kind::Literal }
    , type { type }
    , value { value }
{
}

Member::Member(std::vector<std::string_view> identifiers)
    : Root { Kind::Member }
    , identifiers { std::move(identifiers) }
{
}

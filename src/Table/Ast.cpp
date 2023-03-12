//
// Created by Albert on 05/03/2023.
//
#include "Ast.hpp"

#include <utility>
using namespace templater::table::ast;

Content::Content(SourceLoc loc, List<Statement> statements)
    : Root { Kind::StatementList, loc }
    , m_statements { std::move(statements) }
{
}

Import::Import(SourceLoc loc, std::string_view file, std::string_view identifier)
    : Statement { Kind::Import, loc }
    , m_file { file }
    , m_identifier { identifier }
{
}

Table::Table(SourceLoc loc, std::string_view identifier, List<TableColumn> columns, List<TableContent> content)
    : Statement { Kind::Table, loc }
    , m_identifier { identifier }
    , m_columns { std::move(columns) }
    , m_content { std::move(content) }
{
}

TableColumn::TableColumn(SourceLoc loc, std::string_view identifier, TableValue* value)
    : Root { Kind::TableColumn, loc }
    , m_identifier { identifier }
    , m_value { value }
{
}

TableInherit::TableInherit(SourceLoc loc, Member* member, Expression* expression)
    : TableContent { Kind::TableInherit, loc }
    , m_member { member }
    , m_expression { expression }
{
}

TableBody::TableBody(SourceLoc loc, List<TableRow> rows)
    : TableContent { Kind::TableBody, loc }
    , m_rows { std::move(rows) }
{
}

TableRow::TableRow(SourceLoc loc, List<TableValue> values)
    : Root { Kind::TableRow, loc }
    , m_values { std::move(values) }
{
}

TableValue::TableValue(SourceLoc loc, Literal* literal)
    : Expression { Kind::TableValue, loc }
    , m_value { literal }
{
}

TableValue::TableValue(SourceLoc loc, StructBody* body)
    : Expression { Kind::TableValue, loc }
    , m_value { body }
{
}

UnaryExpression::UnaryExpression(SourceLoc loc, TokenKind type, Expression* rhs)
    : Expression { Kind::UnaryExpression, loc }
    , m_type { type }
    , m_rhs { rhs }
{
}

BinaryExpression::BinaryExpression(SourceLoc loc, TokenKind type, Expression* lhs, Expression* rhs)
    : Expression { Kind::BinaryExpression, loc }
    , m_type { type }
    , m_lhs { lhs }
    , m_rhs { rhs }
{
}

Literal::Literal(SourceLoc loc, TokenKind type, std::string_view value)
    : Root { Kind::Literal, loc }
    , m_type { type }
    , m_value { value }
{
}

Member::Member(SourceLoc loc, std::pmr::vector<std::string_view> identifiers)
    : Root { Kind::Member, loc }
    , m_identifiers { std::move(identifiers) }
{
}

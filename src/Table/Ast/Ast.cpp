//
// Created by Albert on 18/03/2023.
//
#include "Ast.hpp"
using namespace table::ast; // NOLINT

Content::Content(support::SourceLoc loc, support::Source* source, List<std::variant<Node<Import>, Node<Table>>> statements) noexcept
    : Root(loc)
    , m_source(source)
    , m_statements(std::move(statements))
{
}
Content::~Content() = default;

Import::Import(support::SourceLoc loc, table::Identifier identifier, table::StringLiteral file) noexcept
    : Root(loc)
    , m_identifier(identifier)
    , m_file(file)
{
}
Import::~Import() = default;

Table::Table(support::SourceLoc loc, table::Identifier identifier, List<Node<TableColumn>> columns, List<std::variant<Node<TableInherit>, Node<TableBody>>> content) noexcept
    : Root(loc)
    , m_identifier(identifier)
    , m_columns(std::move(columns))
    , m_content(std::move(content))
{
}
Table::~Table() = default;

TableColumn::TableColumn(support::SourceLoc loc, table::Identifier identifier, std::optional<table::Value> value) noexcept
    : Root(loc)
    , m_identifier(identifier)
    , m_value(value)
{
}
TableColumn::~TableColumn() = default;

TableInherit::TableInherit(support::SourceLoc loc, Node<Member> member, std::optional<std::variant<Node<Member>, table::StringLiteral, table::NumberLiteral, Node<UnaryExpression>, Node<BinaryExpression>>> expression) noexcept
    : Root(loc)
    , m_member(std::move(member))
    , m_expression(std::move(expression))
{
}
TableInherit::~TableInherit() = default;

TableBody::TableBody(support::SourceLoc loc, List<Node<TableRow>> rows) noexcept
    : Root(loc)
    , m_rows(std::move(rows))
{
}
TableBody::~TableBody() = default;

TableRow::TableRow(support::SourceLoc loc, List<std::variant<std::monostate, PipeLiteral, table::Value>> values) noexcept
    : Root(loc)
    , m_values(std::move(values))
{
}
TableRow::~TableRow() = default;

UnaryExpression::UnaryExpression(support::SourceLoc loc, Operation op, std::variant<Node<Member>, table::StringLiteral, table::NumberLiteral, Node<UnaryExpression>, Node<BinaryExpression>> rhs) noexcept
    : Root(loc)
    , m_op(op)
    , m_rhs(std::move(rhs))
{
}
UnaryExpression::~UnaryExpression() = default;

BinaryExpression::BinaryExpression(support::SourceLoc loc, Operation op, std::variant<Node<Member>, table::StringLiteral, table::NumberLiteral, Node<UnaryExpression>, Node<BinaryExpression>> lhs, std::variant<Node<Member>, table::StringLiteral, table::NumberLiteral, Node<UnaryExpression>, Node<BinaryExpression>> rhs) noexcept
    : Root(loc)
    , m_op(op)
    , m_lhs(std::move(lhs))
    , m_rhs(std::move(rhs))
{
}
BinaryExpression::~BinaryExpression() = default;

Member::Member(support::SourceLoc loc, List<table::Identifier> identifiers) noexcept
    : Root(loc)
    , m_identifiers(std::move(identifiers))
{
}
Member::~Member() = default;

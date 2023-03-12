//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater::table {
enum class TokenKind;
}

namespace templater::table::ast {

// Ast kinds
enum class Kind {
    StatementList,
    Import,
    Table,
    TableColumn,
    TableInherit,
    TableBody,
    TableRow,
    TableValue,
    StructBody,
    UnaryExpression,
    BinaryExpression,
    Literal,
    Member
};

// Helpers to avoid long type names

template <typename T>
using Node = std::unique_ptr<T>;

template <typename T>
using List = std::vector<Node<T>>;

template <class T, class... Args>
inline Node<T> make(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// Forward

struct TableColumn;
struct TableContent;
struct TableRow;
struct TableValue;
struct StructBody;
struct Literal;
struct Member;

// Basic

struct Root {
    explicit Root(Kind kind)
        : kind { kind }
    {
    }
    Kind kind;
};

struct Statement : Root {
    using Root::Root;
};

struct Expression : Root {
    using Root::Root;
};

struct StatementList final : Root {
    explicit StatementList(List<Statement> statements);

    List<Statement> statements;
};

// Import

struct Import final : Statement {
    Import(std::string_view file, std::string_view identifier);

    std::string_view file;
    std::string_view identifier;
};

// Table

struct Table final : Statement {
    Table(std::string_view identifier, List<TableColumn> columns, List<TableContent> content);

    std::string_view identifier;
    List<TableColumn> columns;
    List<TableContent> content;
};

struct TableColumn final : Root {
    TableColumn(std::string_view identifier, Node<TableValue> value);

    std::string_view identifier;
    Node<TableValue> value;
};

struct TableContent : Root {
    using Root::Root;
};

struct TableInherit final : TableContent {
    TableInherit(Node<Member> member, Node<Expression> expression);

    Node<Member> member;
    Node<Expression> expression;
};

struct TableBody final : TableContent {
    explicit TableBody(List<TableRow> rows);

    List<TableRow> rows;
};

struct TableRow final : Root {
    explicit TableRow(List<TableValue> values);

    List<TableValue> values;
};

struct TableValue final : Expression {
    explicit TableValue(Node<Literal> literal);
    explicit TableValue(Node<StructBody> literal);

    using Value = std::variant<Node<Literal>, Node<StructBody>>;
    Value value;
};

// Structs

struct StructBody final : Root {
    StructBody()
        : Root { Kind::StructBody }
    {
    }
};

// Expressions

struct UnaryExpression final : Expression {
    UnaryExpression(TokenKind type, Node<Expression> rhs);

    TokenKind type;
    Node<Expression> rhs;
};

struct BinaryExpression final : Expression {
    BinaryExpression(TokenKind type, Node<Expression> lhs, Node<Expression> rhs);

    TokenKind type;
    Node<Expression> lhs, rhs;
};

// Misc

struct Literal final : Root {
    Literal(TokenKind type, std::string_view value);

    TokenKind type;
    std::string_view value;
};

struct Member final : Root {
    explicit Member(std::vector<std::string_view> identifiers);

    std::vector<std::string_view> identifiers;
};

} // namespace templater::table::ast

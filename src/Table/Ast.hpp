//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/SourceLoc.hpp"

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
    Root(Kind kind, SourceLoc loc)
        : kind { kind }
        , loc { loc }
    {
    }
    Kind kind;
    SourceLoc loc;
};

struct Statement : Root {
    using Root::Root;
};

struct Expression : Root {
    using Root::Root;
};

struct StatementList final : Root {
    explicit StatementList(SourceLoc loc, List<Statement> statements);

    List<Statement> statements;
};

// Import

struct Import final : Statement {
    Import(SourceLoc loc, std::string_view file, std::string_view identifier);

    std::string_view file;
    std::string_view identifier;
};

// Table

struct Table final : Statement {
    Table(SourceLoc loc, std::string_view identifier, List<TableColumn> columns, List<TableContent> content);

    std::string_view identifier;
    List<TableColumn> columns;
    List<TableContent> content;
};

struct TableColumn final : Root {
    TableColumn(SourceLoc loc, std::string_view identifier, Node<TableValue> value);

    std::string_view identifier;
    Node<TableValue> value;
};

struct TableContent : Root {
    using Root::Root;
};

struct TableInherit final : TableContent {
    TableInherit(SourceLoc loc, Node<Member> member, Node<Expression> expression);

    Node<Member> member;
    Node<Expression> expression;
};

struct TableBody final : TableContent {
    explicit TableBody(SourceLoc loc, List<TableRow> rows);

    List<TableRow> rows;
};

struct TableRow final : Root {
    explicit TableRow(SourceLoc loc, List<TableValue> values);

    List<TableValue> values;
};

struct TableValue final : Expression {
    explicit TableValue(SourceLoc loc, Node<Literal> literal);
    explicit TableValue(SourceLoc loc, Node<StructBody> literal);

    using Value = std::variant<Node<Literal>, Node<StructBody>>;
    Value value;
};

// Structs

struct StructBody final : Root {
    explicit StructBody(SourceLoc loc)
        : Root { Kind::StructBody, loc }
    {
    }
};

// Expressions

struct UnaryExpression final : Expression {
    UnaryExpression(SourceLoc loc, TokenKind type, Node<Expression> rhs);

    TokenKind type;
    Node<Expression> rhs;
};

struct BinaryExpression final : Expression {
    BinaryExpression(SourceLoc loc, TokenKind type, Node<Expression> lhs, Node<Expression> rhs);

    TokenKind type;
    Node<Expression> lhs, rhs;
};

// Misc

struct Literal final : Root {
    Literal(SourceLoc loc, TokenKind type, std::string_view value);

    TokenKind type;
    std::string_view value;
};

struct Member final : Root {
    explicit Member(SourceLoc loc, std::vector<std::string_view> identifiers);

    std::vector<std::string_view> identifiers;
};

} // namespace templater::table::ast

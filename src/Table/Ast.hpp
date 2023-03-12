//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/Context.hpp"
#include "Support/SourceLoc.hpp"

namespace templater::table {
enum class TokenKind : uint8_t;
}

namespace templater::table::ast {

// Ast kinds
enum class Kind : uint8_t {
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
using List = std::pmr::vector<T*>;

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
    TableColumn(SourceLoc loc, std::string_view identifier, TableValue* value);

    std::string_view identifier;
    TableValue* value;
};

struct TableContent : Root {
    using Root::Root;
};

struct TableInherit final : TableContent {
    TableInherit(SourceLoc loc, Member* member, Expression* expression);

    Member* member;
    Expression* expression;
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
    explicit TableValue(SourceLoc loc, Literal* literal);
    explicit TableValue(SourceLoc loc, StructBody* literal);

    using Value = std::variant<Literal*, StructBody*>;
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
    UnaryExpression(SourceLoc loc, TokenKind type, Expression* rhs);

    TokenKind type;
    Expression* rhs;
};

struct BinaryExpression final : Expression {
    BinaryExpression(SourceLoc loc, TokenKind type, Expression* lhs, Expression* rhs);

    TokenKind type;
    Expression *lhs, *rhs;
};

// Misc

struct Literal final : Root {
    Literal(SourceLoc loc, TokenKind type, std::string_view value);

    TokenKind type;
    std::string_view value;
};

struct Member final : Root {
    explicit Member(SourceLoc loc, std::pmr::vector<std::string_view> identifiers);

    std::pmr::vector<std::string_view> identifiers;
};

} // namespace templater::table::ast

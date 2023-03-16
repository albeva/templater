//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/Context.hpp"
#include "Support/SourceLoc.hpp"
#include "Table/Parse/Token.hpp"
#include "Table/Value.hpp"
namespace templater::table::ast {
struct Content;
struct Import;
struct Table;
struct TableColumn;
struct TableInherit;
struct TableBody;
struct TableRow;
struct Operation;
struct UnaryExpression;
struct BinaryExpression;
struct Member;

using Statement = std::variant<Import*, Table*>;
using Expression = std::variant<Identifier, NumberLiteral, StringLiteral, UnaryExpression*, BinaryExpression*>;
using TableContent = std::variant<TableInherit*, TableBody*>;

template <typename T>
using List = std::pmr::vector<T>;

//--------------------------------------
// Root
//--------------------------------------

struct Root {
    explicit constexpr Root(support::SourceLoc loc)
        : m_loc(loc)
    {
    }

    [[nodiscard]] constexpr auto getLoc() const -> const auto& { return m_loc; }

private:
    support::SourceLoc m_loc;
};

//--------------------------------------
// Content
//--------------------------------------

struct Content final : Root {
    constexpr Content(support::SourceLoc loc, List<Statement> statements)
        : Root(loc)
        , m_statements(std::move(statements))
    {
    }

    [[nodiscard]] constexpr auto getStatements() const -> auto& { return m_statements; }

private:
    List<Statement> m_statements;
};

struct Import final : Root {
    constexpr Import(support::SourceLoc loc, Identifier identifier, StringLiteral file)
        : Root(loc)
        , m_identifier(identifier)
        , m_file(file)
    {
    }

    [[nodiscard]] constexpr auto getFile() const -> auto& { return m_file; }
    [[nodiscard]] constexpr auto getIdentifier() const -> auto& { return m_identifier; }

private:
    Identifier m_identifier;
    StringLiteral m_file;
};

//--------------------------------------
// Table
//--------------------------------------

struct Table final : Root {
    constexpr Table(support::SourceLoc loc, Identifier identifier, List<TableColumn*> columns, List<TableContent> content)
        : Root(loc)
        , m_identifier(identifier)
        , m_columns(std::move(columns))
        , m_content(std::move(content))
    {
    }

    [[nodiscard]] constexpr auto getIdentifier() const -> const auto& { return m_identifier; }
    [[nodiscard]] constexpr auto getColumns() const -> const auto& { return m_columns; }
    [[nodiscard]] constexpr auto getContent() const -> const auto& { return m_content; }

private:
    Identifier m_identifier;
    List<TableColumn*> m_columns;
    List<TableContent> m_content;
};

struct TableColumn final : Root {
    constexpr TableColumn(support::SourceLoc loc, Identifier identifier, std::optional<Value> value)
        : Root(loc)
        , m_identifier(identifier)
        , m_value(value)
    {
    }

    [[nodiscard]] constexpr auto getIdentifier() const -> const auto& { return m_identifier; }
    [[nodiscard]] constexpr auto getValue() const -> const auto& { return m_value; }

private:
    Identifier m_identifier;
    std::optional<Value> m_value;
};

struct TableInherit final : Root {
    constexpr TableInherit(support::SourceLoc loc, Member* member, std::optional<Expression> expression)
        : Root(loc)
        , m_member(member)
        , m_expression(expression)
    {
    }

    [[nodiscard]] constexpr auto getMember() const -> const auto& { return m_member; }
    [[nodiscard]] constexpr auto getExpression() const -> const auto& { return m_expression; }

private:
    Member* m_member;
    std::optional<Expression> m_expression;
};

struct TableBody final : Root {
    constexpr TableBody(support::SourceLoc loc, List<TableRow*> rows)
        : Root(loc)
        , m_rows(std::move(rows))
    {
    }

    [[nodiscard]] constexpr auto getRows() const -> const auto& { return m_rows; }

private:
    List<TableRow*> m_rows;
};

struct TableRow final : Root {
    constexpr TableRow(support::SourceLoc loc, List<std::optional<Value>> values)
        : Root(loc)
        , m_values(std::move(values))
    {
    }

    [[nodiscard]] constexpr auto getValues() const -> const auto& { return m_values; }

private:
    List<std::optional<Value>> m_values;
};

//--------------------------------------
// Expressions
//--------------------------------------

struct Operation final : Root {
    constexpr Operation(support::SourceLoc loc, parser::TokenKind kind)
        : Root(loc)
        , m_kind(kind)
    {
    }

    [[nodiscard]] constexpr auto getKind() const -> const auto& { return m_kind; }

private:
    parser::TokenKind m_kind;
};

struct UnaryExpression final : Root {
    constexpr UnaryExpression(support::SourceLoc loc, Operation op, Expression rhs)
        : Root(loc)
        , m_op(op)
        , m_rhs(rhs)
    {
    }

    [[nodiscard]] constexpr auto getOp() const -> const auto& { return m_op; }
    [[nodiscard]] constexpr auto getRhs() const -> const auto& { return m_rhs; }

private:
    Operation m_op;
    Expression m_rhs;
};

struct BinaryExpression final : Root {
    constexpr BinaryExpression(support::SourceLoc loc, Operation op, Expression lhs, Expression rhs)
        : Root(loc)
        , m_op(op)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    [[nodiscard]] constexpr auto getOp() const -> const auto& { return m_op; }
    [[nodiscard]] constexpr auto getLhs() const -> const auto& { return m_lhs; }
    [[nodiscard]] constexpr auto getRhs() const -> const auto& { return m_rhs; }

private:
    Operation m_op;
    Expression m_lhs, m_rhs;
};

//--------------------------------------
// Misc
//--------------------------------------

struct Member final : Root {
    constexpr Member(support::SourceLoc loc, List<Identifier> identifiers)
        : Root(loc)
        , m_identifiers(std::move(identifiers))
    {
    }

    [[nodiscard]] constexpr auto getIdentifiers() const -> const auto& { return m_identifiers; }

private:
    List<Identifier> m_identifiers;
};

} // namespace templater::table::ast

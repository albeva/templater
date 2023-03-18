//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/Context.hpp"
#include "Support/SourceLoc.hpp"
#include "Table/Parse/Token.hpp"
#include "Table/Value.hpp"
namespace support {
class Source;
}
namespace table::ast {
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

template <typename T>
using List = std::pmr::vector<T>;

using Statement = std::variant<Import*, Table*>;
using Expression = std::variant<Member*, StringLiteral, NumberLiteral, UnaryExpression*, BinaryExpression*>;
using TableContent = std::variant<TableInherit*, TableBody*>;
struct PipeLiteral { };
using TableValue = std::variant<std::monostate, PipeLiteral, Value>;

//--------------------------------------
// Root
//--------------------------------------

struct Root {
    explicit Root(support::SourceLoc loc) noexcept
        : m_loc(loc)
    {
    }

    [[nodiscard]] inline auto getLoc() const -> const auto& { return m_loc; }

    NO_NEW_AND_DELETE
private:
    support::SourceLoc m_loc;
};

//--------------------------------------
// Content
//--------------------------------------

struct Content final : Root {
    Content(support::SourceLoc loc, support::Source* source, List<Statement> statements) noexcept
        : Root(loc)
        , m_source(source)
        , m_statements(std::move(statements))
    {
    }

    [[nodiscard]] inline auto getSource() const noexcept -> const auto& { return m_source; }
    [[nodiscard]] inline auto getStatements() const noexcept -> const auto& { return m_statements; }

private:
    support::Source* m_source;
    List<Statement> m_statements;
};

struct Import final : Root {
    Import(support::SourceLoc loc, Identifier identifier, StringLiteral file) noexcept
        : Root(loc)
        , m_identifier(identifier)
        , m_file(file)
    {
    }

    [[nodiscard]] inline auto getFile() const noexcept -> const auto& { return m_file; }
    [[nodiscard]] inline auto getIdentifier() const noexcept -> const auto& { return m_identifier; }

private:
    Identifier m_identifier;
    StringLiteral m_file;
};

//--------------------------------------
// Table
//--------------------------------------

struct Table final : Root {
    Table(support::SourceLoc loc, Identifier identifier, List<TableColumn*> columns, List<TableContent> content) noexcept
        : Root(loc)
        , m_identifier(identifier)
        , m_columns(std::move(columns))
        , m_content(std::move(content))
    {
    }

    [[nodiscard]] inline auto getIdentifier() const noexcept -> const auto& { return m_identifier; }
    [[nodiscard]] inline auto getColumns() const noexcept -> const auto& { return m_columns; }
    [[nodiscard]] inline auto getContent() const noexcept -> const auto& { return m_content; }

private:
    Identifier m_identifier;
    List<TableColumn*> m_columns;
    List<TableContent> m_content;
};

struct TableColumn final : Root {
    TableColumn(support::SourceLoc loc, Identifier identifier, std::optional<Value> value) noexcept
        : Root(loc)
        , m_identifier(identifier)
        , m_value(value)
    {
    }

    [[nodiscard]] inline auto getIdentifier() const noexcept -> const auto& { return m_identifier; }
    [[nodiscard]] inline auto getValue() const noexcept -> const auto& { return m_value; }

private:
    Identifier m_identifier;
    std::optional<Value> m_value;
};

struct TableInherit final : Root {
    TableInherit(support::SourceLoc loc, Member* member, std::optional<Expression> expression) noexcept
        : Root(loc)
        , m_member(member)
        , m_expression(expression)
    {
    }

    [[nodiscard]] inline auto getMember() const noexcept -> const auto& { return m_member; }
    [[nodiscard]] inline auto getExpression() const noexcept -> const auto& { return m_expression; }

private:
    Member* m_member;
    std::optional<Expression> m_expression;
};

struct TableBody final : Root {
    TableBody(support::SourceLoc loc, List<TableRow*> rows) noexcept
        : Root(loc)
        , m_rows(std::move(rows))
    {
    }

    [[nodiscard]] inline auto getRows() const noexcept -> const auto& { return m_rows; }

private:
    List<TableRow*> m_rows;
};

struct TableRow final : Root {
    TableRow(support::SourceLoc loc, List<TableValue> values) noexcept
        : Root(loc)
        , m_values(std::move(values))
    {
    }

    [[nodiscard]] inline auto getValues() const noexcept -> const auto& { return m_values; }

private:
    List<TableValue> m_values;
};

//--------------------------------------
// Expressions
//--------------------------------------

// This node is held by value
struct Operation final : Root {
    Operation(support::SourceLoc loc, parser::TokenKind kind) noexcept
        : Root(loc)
        , m_kind(kind)
    {
    }

    [[nodiscard]] inline auto getKind() const noexcept -> const auto& { return m_kind; }

private:
    parser::TokenKind m_kind;
};

struct UnaryExpression final : Root {
    UnaryExpression(support::SourceLoc loc, Operation op, Expression rhs) noexcept
        : Root(loc)
        , m_op(op)
        , m_rhs(rhs)
    {
    }

    [[nodiscard]] inline auto getOp() const noexcept -> const auto& { return m_op; }
    [[nodiscard]] inline auto getRhs() const noexcept -> const auto& { return m_rhs; }

private:
    Operation m_op;
    Expression m_rhs;
};

struct BinaryExpression final : Root {
    BinaryExpression(support::SourceLoc loc, Operation op, Expression lhs, Expression rhs) noexcept
        : Root(loc)
        , m_op(op)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    [[nodiscard]] inline auto getOp() const noexcept -> const auto& { return m_op; }
    [[nodiscard]] inline auto getLhs() const noexcept -> const auto& { return m_lhs; }
    [[nodiscard]] inline auto getRhs() const noexcept -> const auto& { return m_rhs; }

private:
    Operation m_op;
    Expression m_lhs, m_rhs;
};

//--------------------------------------
// Misc
//--------------------------------------

struct Member final : Root {
    Member(support::SourceLoc loc, List<Identifier> identifiers) noexcept
        : Root(loc)
        , m_identifiers(std::move(identifiers))
    {
    }

    [[nodiscard]] inline auto getIdentifiers() const noexcept -> const auto& { return m_identifiers; }

private:
    List<Identifier> m_identifiers;
};

} // namespace table::ast

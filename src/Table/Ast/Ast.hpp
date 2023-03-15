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
struct StructBody;
struct UnaryExpression;
struct BinaryExpression;
struct Member;

using Statement = std::variant<Import*, Table*>;
using Expression = std::variant<Identifier, NumberLiteral, StringLiteral, UnaryExpression*, BinaryExpression*>;
using TableValue = std::variant<Identifier, NumberLiteral, StringLiteral, StructBody*>;
using TableContent = std::variant<TableInherit*, TableBody*>;

template <typename T>
using List = std::pmr::vector<T>;

//--------------------------------------
// Root
//--------------------------------------

struct Root {
    explicit Root(support::SourceLoc loc)
        : m_loc(loc)
    {
    }

    [[nodiscard]] inline auto getLoc() const -> const auto& { return m_loc; }

private:
    support::SourceLoc m_loc;
};

//--------------------------------------
// Content
//--------------------------------------

struct Content final : Root {
    explicit Content(support::SourceLoc loc, List<Statement> statements);

    [[nodiscard]] auto getStatements() const -> auto& { return m_statements; }

private:
    List<Statement> m_statements;
};

struct Import final : Root {
    Import(support::SourceLoc loc, Identifier identifier, StringLiteral file);

    [[nodiscard]] auto getFile() const { return m_file; }
    [[nodiscard]] auto getIdentifier() const { return m_identifier; }

private:
    Identifier m_identifier;
    StringLiteral m_file;
};

//--------------------------------------
// Table
//--------------------------------------

struct Table final : Root {
    Table(support::SourceLoc loc, Identifier identifier, List<TableColumn*> columns, List<TableContent> content);

    [[nodiscard]] auto getIdentifier() const -> const auto& { return m_identifier; }
    [[nodiscard]] auto getColumns() const -> auto& { return m_columns; }
    [[nodiscard]] auto getContent() const -> auto& { return m_content; }

private:
    Identifier m_identifier;
    List<TableColumn*> m_columns;
    List<TableContent> m_content;
};

struct TableColumn final : Root {
    TableColumn(support::SourceLoc loc, Identifier identifier, std::optional<TableValue> value);

    [[nodiscard]] auto getIdentifier() const -> const auto& { return m_identifier; }
    [[nodiscard]] auto getValue() const { return m_value; }

private:
    Identifier m_identifier;
    std::optional<TableValue> m_value;
};

struct TableInherit final : Root {
    TableInherit(support::SourceLoc loc, Member* member, std::optional<Expression> expression);

    [[nodiscard]] auto getMember() const { return m_member; }
    [[nodiscard]] auto getExpression() const { return m_expression; }

private:
    Member* m_member;
    std::optional<Expression> m_expression;
};

struct TableBody final : Root {
    explicit TableBody(support::SourceLoc loc, List<TableRow*> rows);

    [[nodiscard]] auto getRows() const -> auto& { return m_rows; }

private:
    List<TableRow*> m_rows;
};

struct TableRow final : Root {
    explicit TableRow(support::SourceLoc loc, List<TableValue> values);

    [[nodiscard]] auto getValues() const -> auto& { return m_values; }

private:
    List<TableValue> m_values;
};

//--------------------------------------
// Structs
//--------------------------------------

struct StructBody final : Root {
    explicit StructBody(support::SourceLoc loc)
        : Root { loc }
    {
    }
};

//--------------------------------------
// Expressions
//--------------------------------------

struct Operation final : Root {
    Operation(support::SourceLoc loc, parser::TokenKind kind)
        : Root(loc)
        , m_kind(kind)
    {
    }

    [[nodiscard]] auto getKind() const { return m_kind; }

private:
    parser::TokenKind m_kind;
};

struct UnaryExpression final : Root {
    UnaryExpression(support::SourceLoc loc, Operation op, Expression rhs);

    [[nodiscard]] auto getOp() const { return m_op; }
    [[nodiscard]] auto getRhs() const { return m_rhs; }

private:
    Operation m_op;
    Expression m_rhs;
};

struct BinaryExpression final : Root {
    BinaryExpression(support::SourceLoc loc, Operation op, Expression lhs, Expression rhs);

    [[nodiscard]] auto getOp() const { return m_op; }
    [[nodiscard]] auto getLhs() const { return m_lhs; }
    [[nodiscard]] auto getRhs() const { return m_rhs; }

private:
    Operation m_op;
    Expression m_lhs, m_rhs;
};

//--------------------------------------
// Misc
//--------------------------------------

struct Member final : Root {
    explicit Member(support::SourceLoc loc, List<Identifier> identifiers);

    [[nodiscard]] auto getIdentifiers() const -> auto& { return m_identifiers; }

private:
    List<Identifier> m_identifiers;
};

} // namespace templater::table::ast

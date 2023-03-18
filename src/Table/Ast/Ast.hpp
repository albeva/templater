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
    NO_COPY_AND_MOVE(Content)
    Content(support::SourceLoc loc, support::Source* source, List<Statement> statements) noexcept;
    ~Content();

    [[nodiscard]] inline auto getSource() const noexcept -> const auto& { return m_source; }
    [[nodiscard]] inline auto getStatements() const noexcept -> const auto& { return m_statements; }

private:
    support::Source* m_source;
    List<Statement> m_statements;
};

struct Import final : Root {
    NO_COPY_AND_MOVE(Import)
    Import(support::SourceLoc loc, Identifier identifier, StringLiteral file) noexcept;
    ~Import();

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
    NO_COPY_AND_MOVE(Table)
    Table(support::SourceLoc loc, Identifier identifier, List<TableColumn*> columns, List<TableContent> content) noexcept;
    ~Table();

    [[nodiscard]] inline auto getIdentifier() const noexcept -> const auto& { return m_identifier; }
    [[nodiscard]] inline auto getColumns() const noexcept -> const auto& { return m_columns; }
    [[nodiscard]] inline auto getContent() const noexcept -> const auto& { return m_content; }

private:
    Identifier m_identifier;
    List<TableColumn*> m_columns;
    List<TableContent> m_content;
};

struct TableColumn final : Root {
    NO_COPY_AND_MOVE(TableColumn)
    TableColumn(support::SourceLoc loc, Identifier identifier, std::optional<Value> value) noexcept;
    ~TableColumn();

    [[nodiscard]] inline auto getIdentifier() const noexcept -> const auto& { return m_identifier; }
    [[nodiscard]] inline auto getValue() const noexcept -> const auto& { return m_value; }

private:
    Identifier m_identifier;
    std::optional<Value> m_value;
};

struct TableInherit final : Root {
    NO_COPY_AND_MOVE(TableInherit)
    TableInherit(support::SourceLoc loc, Member* member, std::optional<Expression> expression) noexcept;
    ~TableInherit();

    [[nodiscard]] inline auto getMember() const noexcept -> const auto& { return m_member; }
    [[nodiscard]] inline auto getExpression() const noexcept -> const auto& { return m_expression; }

private:
    Member* m_member;
    std::optional<Expression> m_expression;
};

struct TableBody final : Root {
    NO_COPY_AND_MOVE(TableBody)
    TableBody(support::SourceLoc loc, List<TableRow*> rows) noexcept;
    ~TableBody();

    [[nodiscard]] inline auto getRows() const noexcept -> const auto& { return m_rows; }

private:
    List<TableRow*> m_rows;
};

struct TableRow final : Root {
    NO_COPY_AND_MOVE(TableRow)
    TableRow(support::SourceLoc loc, List<TableValue> values) noexcept;
    ~TableRow();

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
    NO_COPY_AND_MOVE(UnaryExpression)
    UnaryExpression(support::SourceLoc loc, Operation op, Expression rhs) noexcept;
    ~UnaryExpression();

    [[nodiscard]] inline auto getOp() const noexcept -> const auto& { return m_op; }
    [[nodiscard]] inline auto getRhs() const noexcept -> const auto& { return m_rhs; }

private:
    Operation m_op;
    Expression m_rhs;
};

struct BinaryExpression final : Root {
    NO_COPY_AND_MOVE(BinaryExpression)
    BinaryExpression(support::SourceLoc loc, Operation op, Expression lhs, Expression rhs) noexcept;
    ~BinaryExpression();

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
    NO_COPY_AND_MOVE(Member)
    Member(support::SourceLoc loc, List<Identifier> identifiers) noexcept;
    ~Member();

    [[nodiscard]] inline auto getIdentifiers() const noexcept -> const auto& { return m_identifiers; }

private:
    List<Identifier> m_identifiers;
};

} // namespace table::ast

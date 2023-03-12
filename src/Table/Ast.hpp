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
        : m_kind { kind }
        , m_loc { loc }
    {
    }

    [[nodiscard]] auto getKind() const { return m_kind; }
    [[nodiscard]] auto getLoc() const { return m_loc; }

private:
    Kind m_kind;
    SourceLoc m_loc;
};

struct Statement : Root {
    using Root::Root;
};

struct Expression : Root {
    using Root::Root;
};

struct StatementList final : Root {
    explicit StatementList(SourceLoc loc, List<Statement> statements);

    [[nodiscard]] auto getStatements() const -> auto& { return m_statements; }

private:
    List<Statement> m_statements;
};

// Import

struct Import final : Statement {
    Import(SourceLoc loc, std::string_view file, std::string_view identifier);

    [[nodiscard]] auto getFile() const { return m_file; }
    [[nodiscard]] auto getIdentifier() const { return m_identifier; }

private:
    std::string_view m_file;
    std::string_view m_identifier;
};

// Table

struct Table final : Statement {
    Table(SourceLoc loc, std::string_view identifier, List<TableColumn> columns, List<TableContent> content);

    [[nodiscard]] auto getIdentifier() const { return m_identifier; }
    [[nodiscard]] auto getColumns() const -> auto& { return m_columns; }
    [[nodiscard]] auto getContent() const -> auto& { return m_content; }

private:
    std::string_view m_identifier;
    List<TableColumn> m_columns;
    List<TableContent> m_content;
};

struct TableColumn final : Root {
    TableColumn(SourceLoc loc, std::string_view identifier, TableValue* value);

    [[nodiscard]] auto getIdentifier() const { return m_identifier; }
    [[nodiscard]] auto getValue() const { return m_value; }

private:
    std::string_view m_identifier;
    TableValue* m_value;
};

struct TableContent : Root {
    using Root::Root;
};

struct TableInherit final : TableContent {
    TableInherit(SourceLoc loc, Member* member, Expression* expression);

    [[nodiscard]] auto getMember() const { return m_member; }
    [[nodiscard]] auto getExpression() const { return m_expression; }

private:
    Member* m_member;
    Expression* m_expression;
};

struct TableBody final : TableContent {
    explicit TableBody(SourceLoc loc, List<TableRow> rows);

    [[nodiscard]] auto getRows() const -> auto& { return m_rows; }

private:
    List<TableRow> m_rows;
};

struct TableRow final : Root {
    explicit TableRow(SourceLoc loc, List<TableValue> values);

    [[nodiscard]] auto getValues() const -> auto& { return m_values; }

private:
    List<TableValue> m_values;
};

struct TableValue final : Expression {
    explicit TableValue(SourceLoc loc, Literal* literal);
    explicit TableValue(SourceLoc loc, StructBody* body);

    using Value = std::variant<Literal*, StructBody*>;

    [[nodiscard]] auto getValue() const { return m_value; }

private:
    Value m_value;
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

    [[nodiscard]] auto getType() const { return m_type; }
    [[nodiscard]] auto getRhs() const { return m_rhs; }

private:
    TokenKind m_type;
    Expression* m_rhs;
};

struct BinaryExpression final : Expression {
    BinaryExpression(SourceLoc loc, TokenKind type, Expression* lhs, Expression* rhs);

    [[nodiscard]] auto getType() const { return m_type; }
    [[nodiscard]] auto getLhs() const { return m_lhs; }
    [[nodiscard]] auto getRhs() const { return m_rhs; }

private:
    TokenKind m_type;
    Expression *m_lhs, *m_rhs;
};

// Misc

struct Literal final : Root {
    Literal(SourceLoc loc, TokenKind type, std::string_view value);

    [[nodiscard]] auto getType() const { return m_type; }
    [[nodiscard]] auto getValue() const { return m_value; }

private:
    TokenKind m_type;
    std::string_view m_value;
};

struct Member final : Root {
    explicit Member(SourceLoc loc, std::pmr::vector<std::string_view> identifiers);

    [[nodiscard]] auto getIdentifiers() const -> auto& { return m_identifiers; }

private:
    std::pmr::vector<std::string_view> m_identifiers;
};

} // namespace templater::table::ast

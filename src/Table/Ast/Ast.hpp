//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/Context.hpp"
#include "Support/SourceLoc.hpp"
#include "Table/Parse/Token.hpp"

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
using Expression = std::variant<parser::Token, UnaryExpression*, BinaryExpression*>;
using TableValue = std::variant<parser::Token, StructBody*>;
using TableContent = std::variant<TableInherit*, TableBody*>;

template <typename T>
using List = std::pmr::vector<T>;

struct Root {
    explicit Root(SourceLoc loc)
        : m_loc(loc)
    {
    }

    [[nodiscard]] auto getLoc() const { return m_loc; }

private:
    SourceLoc m_loc;
};

//--------------------------------------
// Content
//--------------------------------------

struct Content final : Root {
    explicit Content(SourceLoc loc, List<Statement> statements);

    [[nodiscard]] auto getStatements() const -> auto& { return m_statements; }

private:
    List<Statement> m_statements;
};

struct Import final : Root {
    Import(SourceLoc loc, std::string_view file, std::string_view identifier);

    [[nodiscard]] auto getFile() const { return m_file; }
    [[nodiscard]] auto getIdentifier() const { return m_identifier; }

private:
    std::string_view m_file;
    std::string_view m_identifier;
};

//--------------------------------------
// Table
//--------------------------------------

struct Table final : Root {
    Table(SourceLoc loc, parser::Token identifier, List<TableColumn*> columns, List<TableContent> content);

    [[nodiscard]] auto getIdentifier() const -> const auto& { return m_identifier; }
    [[nodiscard]] auto getColumns() const -> auto& { return m_columns; }
    [[nodiscard]] auto getContent() const -> auto& { return m_content; }

private:
    parser::Token m_identifier;
    List<TableColumn*> m_columns;
    List<TableContent> m_content;
};

struct TableColumn final : Root {
    TableColumn(SourceLoc loc, parser::Token identifier, std::optional<TableValue> value);

    [[nodiscard]] auto getIdentifier() const -> const parser::Token& { return m_identifier; }
    [[nodiscard]] auto getValue() const { return m_value; }

private:
    parser::Token m_identifier;
    std::optional<TableValue> m_value;
};

struct TableInherit final : Root {
    TableInherit(SourceLoc loc, Member* member, std::optional<Expression> expression);

    [[nodiscard]] auto getMember() const { return m_member; }
    [[nodiscard]] auto getExpression() const { return m_expression; }

private:
    Member* m_member;
    std::optional<Expression> m_expression;
};

struct TableBody final : Root {
    explicit TableBody(SourceLoc loc, List<TableRow*> rows);

    [[nodiscard]] auto getRows() const -> auto& { return m_rows; }

private:
    List<TableRow*> m_rows;
};

struct TableRow final : Root {
    explicit TableRow(SourceLoc loc, List<TableValue> values);

    [[nodiscard]] auto getValues() const -> auto& { return m_values; }

private:
    List<TableValue> m_values;
};

//--------------------------------------
// Structs
//--------------------------------------

struct StructBody final : Root {
    explicit StructBody(SourceLoc loc)
        : Root { loc }
    {
    }
};

//--------------------------------------
// Expressions
//--------------------------------------

struct UnaryExpression final : Root {
    UnaryExpression(SourceLoc loc, parser::TokenKind type, Expression rhs);

    [[nodiscard]] auto getType() const { return m_type; }
    [[nodiscard]] auto getRhs() const { return m_rhs; }

private:
    parser::TokenKind m_type;
    Expression m_rhs;
};

struct BinaryExpression final : Root {
    BinaryExpression(SourceLoc loc, parser::TokenKind type, Expression lhs, Expression rhs);

    [[nodiscard]] auto getType() const { return m_type; }
    [[nodiscard]] auto getLhs() const { return m_lhs; }
    [[nodiscard]] auto getRhs() const { return m_rhs; }

private:
    parser::TokenKind m_type;
    Expression m_lhs, m_rhs;
};

//--------------------------------------
// Misc
//--------------------------------------

struct Member final : Root {
    explicit Member(SourceLoc loc, std::pmr::vector<std::string_view> identifiers);

    [[nodiscard]] auto getIdentifiers() const -> auto& { return m_identifiers; }

private:
    std::pmr::vector<std::string_view> m_identifiers;
};

} // namespace templater::table::ast

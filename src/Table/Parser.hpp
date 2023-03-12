//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
#include "Token.hpp"

namespace templater::table {
class Lexer;

class ParserException final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Parser final {
public:
    NO_COPY_AND_MOVE(Parser)
    ~Parser() = default;
    explicit Parser(Lexer& lexer);

    [[nodiscard]] ast::Node<ast::StatementList> parse();

private:
    ast::Node<ast::Statement> statement();
    ast::Node<ast::Import> kwImport();

    ast::Node<ast::Table> kwTable();
    ast::List<ast::TableColumn> tableColumnList();
    ast::Node<ast::TableColumn> tableColumn();
    ast::List<ast::TableContent> tableContentList();
    ast::Node<ast::TableContent> tableContent();
    ast::Node<ast::TableInherit> tableInherit();
    ast::Node<ast::TableBody> tableBody();
    ast::List<ast::TableRow> tableRowList();
    ast::Node<ast::TableRow> tableRow();
    ast::Node<ast::TableValue> tableValue();

    ast::Node<ast::Expression> expression();
    ast::Node<ast::Expression> primary();
    ast::Node<ast::Expression> expression(ast::Node<ast::Expression> lhs, int prec);

    ast::Node<ast::Member> member();
    ast::Node<ast::Literal> literal();

    bool accept(TokenKind kind);
    void expect(TokenKind kind);
    [[nodiscard]] std::string_view consume(TokenKind kind);
    void next();

    [[noreturn]] void unexpected(const std::string& message);

    Lexer& m_lexer;
    Token m_token {};
};

} // namespace templater::table
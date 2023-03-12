//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
#include "AstAllocator.hpp"
#include "Support/Source.hpp"
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
    explicit Parser(Context&, Lexer& lexer);

    [[nodiscard]] ast::StatementList* parse();

private:
    ast::Statement* statement();
    ast::Import* kwImport();

    ast::Table* kwTable();
    ast::List<ast::TableColumn> tableColumnList();
    ast::TableColumn* tableColumn();
    ast::List<ast::TableContent> tableContentList();
    ast::TableContent* tableContent();
    ast::TableInherit* tableInherit();
    ast::TableBody* tableBody();
    ast::List<ast::TableRow> tableRowList();
    ast::TableRow* tableRow();
    ast::TableValue* tableValue();

    ast::Expression* expression();
    ast::Expression* primary();
    ast::Expression* expression(ast::Expression* lhs, int prec);

    ast::Member* member();
    ast::Literal* literal();

    bool accept(TokenKind kind);
    void expect(TokenKind kind);
    [[nodiscard]] std::string_view consume(TokenKind kind);
    void next();
    [[nodiscard]] static SourceLoc loc(SourceLoc start, SourceLoc end);

    [[noreturn]] void unexpected(const std::string& message);

    Context& m_ctx;
    Lexer& m_lexer;
    ast::AstAllocator m_ast;
    Token m_token {};
};

} // namespace templater::table
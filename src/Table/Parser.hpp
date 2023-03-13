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
    explicit Parser(Context* ctx, Lexer* lexer);

    [[nodiscard]] auto parse() -> ast::Content*;

private:
    auto statement() -> ast::Statement;
    auto kwImport() -> ast::Import*;

    auto kwTable() -> ast::Table*;
    auto tableColumnList() -> ast::List<ast::TableColumn*>;
    auto tableColumn() -> ast::TableColumn*;
    auto tableContentList() -> ast::List<ast::TableContent>;
    auto tableContent() -> ast::TableContent;
    auto tableInherit() -> ast::TableInherit*;
    auto tableBody() -> ast::TableBody*;
    auto tableRowList() -> ast::List<ast::TableRow*>;
    auto tableRow() -> ast::TableRow*;
    auto tableValue() -> ast::TableValue;

    auto expression() -> ast::Expression;
    auto primary() -> ast::Expression;
    auto expression(ast::Expression lhs, int min) -> ast::Expression;

    auto member() -> ast::Member*;
    auto literal() -> ast::Literal*;

    auto accept(TokenKind kind) -> bool;
    void expect(TokenKind kind);
    [[nodiscard]] auto consume(TokenKind kind) -> std::string_view;
    void next();
    [[nodiscard]] static auto makeLoc(SourceLoc start, SourceLoc end) -> SourceLoc;

    [[noreturn]] void expected(std::string_view message);

    Context* m_ctx;
    Lexer* m_lexer;
    ast::AstAllocator m_ast;
    Token m_token {};
    SourceLoc m_lastLoc {};
};

} // namespace templater::table
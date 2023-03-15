//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/Source.hpp"
#include "Table/Ast/Allocator.hpp"
#include "Table/Ast/Ast.hpp"
#include "Token.hpp"

namespace templater::support {
class Diagnostics;
}

namespace templater::table::parser {
class Lexer;

class ParserException final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Parser final {
public:
    NO_COPY_AND_MOVE(Parser)
    ~Parser() = default;
    explicit Parser(support::Context* ctx, support::Diagnostics* diag, Lexer* lexer);

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

    auto accept(TokenKind kind) -> bool;
    void expect(TokenKind kind);
    void next();
    [[nodiscard]] static auto makeLoc(support::SourceLoc start, support::SourceLoc end) -> support::SourceLoc;

    [[noreturn]] void expected(std::string_view message);

    support::Context* m_ctx;
    support::Diagnostics* m_diag;
    Lexer* m_lexer;
    ast::Allocator m_ast;
    Token m_token {};
    support::SourceLoc m_lastLoc {};
};

} // namespace templater::table::parser
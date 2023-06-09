//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Table/Ast/Ast.hpp"
#include "Table/Value.hpp"
#include "Token.hpp"

namespace support {
class Diagnostics;
}

namespace table::ast {
class Context;
}

namespace table::parser {
class Lexer;

class ParserException final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Parser final {
public:
    NO_COPY_AND_MOVE(Parser)
    ~Parser() = default;
    explicit Parser(support::Diagnostics* diag, Lexer* lexer);

    [[nodiscard]] auto parse() -> std::unique_ptr<ast::Context>;

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

    auto expression() -> ast::Expression;
    auto primary() -> ast::Expression;
    auto expression(ast::Expression lhs, int min) -> ast::Expression;
    auto operation() -> ast::Operation;

    auto member() -> ast::Member*;

    [[nodiscard]] auto value() -> Value;

    auto identifier() -> Identifier;
    auto stringLiteral() -> StringLiteral;
    auto numberLiteral() -> NumberLiteral;

    auto accept(TokenKind kind) -> bool;
    void expect(TokenKind kind);
    void next();
    [[nodiscard]] static auto makeLoc(support::SourceLoc start, support::SourceLoc end) noexcept -> support::SourceLoc;

    [[noreturn]] void expected(std::string_view message);

    support::Diagnostics* m_diag;
    Lexer* m_lexer;
    std::unique_ptr<ast::Context> m_ast;
    Token m_token {};
    support::SourceLoc m_lastLoc {};
};

} // namespace table::parser
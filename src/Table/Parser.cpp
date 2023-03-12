//
// Created by Albert on 05/03/2023.
//
#include "Parser.hpp"
#include "Ast.hpp"
#include "Lexer.hpp"
#include "Support/Context.hpp"

using namespace templater;
using namespace templater::table;

Parser::Parser(Context* ctx, Lexer* lexer)
    : m_ctx { ctx }
    , m_lexer { lexer }
    , m_ast { m_ctx }
{
    next();
}

// { Statement }
auto Parser::parse() -> ast::StatementList*
{
    auto start = m_token.getLoc();

    auto stmtList = m_ast.list<ast::Statement>();
    while (m_token.isNot(TokenKind::EndOfFile)) {
        stmtList.push_back(statement());
        if (!accept(TokenKind::EndOfLine)) {
            break;
        }
    }

    auto end = m_token.getLoc();
    expect(TokenKind::EndOfFile);
    return m_ast.node<ast::StatementList>(loc(start, end), std::move(stmtList));
}

// ( Import | Table )
auto Parser::statement() -> ast::Statement*
{
    ast::Statement* stmt {};

    switch (m_token.getKind()) {
    case TokenKind::KwImport:
        stmt = kwImport();
        break;
    case TokenKind::KwTable:
        stmt = kwTable();
        break;
    default:
        unexpected("Expected 'import' or 'table'");
    }

    return stmt;
}

// "import" STRING "as" IDENTIFIER
auto Parser::kwImport() -> ast::Import*
{
    auto start = m_token.getLoc();
    expect(TokenKind::KwImport);

    auto import = consume(TokenKind::String);
    expect(TokenKind::KwAs);
    auto end = m_token.getLoc();
    auto ident = consume(TokenKind::Identifier);

    return m_ast.node<ast::Import>(loc(start, end), import, ident);
}

//------------------------------------------------------------------------------
// Table
//------------------------------------------------------------------------------

// "table" IDENTIFIER [ "(" TableColumnList ")" ] [ "=" TableContentList ]
auto Parser::kwTable() -> ast::Table*
{
    auto start = m_token.getLoc();
    expect(TokenKind::KwTable);

    auto ident = consume(TokenKind::Identifier);

    auto columns = m_ast.list<ast::TableColumn>();
    if (accept(TokenKind::ParenOpen)) {
        columns = tableColumnList();
        expect(TokenKind::ParenClose);
    }

    expect(TokenKind::Assign);
    auto content = tableContentList();
    auto end = content.back()->getLoc();

    return m_ast.node<ast::Table>(loc(start, end), ident, std::move(columns), std::move(content));
}

// TableColumn { "," TableColumn }
auto Parser::tableColumnList() -> ast::List<ast::TableColumn>
{
    auto columns = m_ast.list<ast::TableColumn>();

    do {
        columns.push_back(tableColumn());
    } while (m_token.isNot(TokenKind::ParenClose));

    return columns;
}

// IDENTIFIER [ "=" TableValue ]
auto Parser::tableColumn() -> ast::TableColumn*
{
    auto start = m_token.getLoc();
    auto ident = consume(TokenKind::Identifier);
    ast::TableValue* value {};
    if (accept(TokenKind::Assign)) {
        value = tableValue();
    }
    auto end = value != nullptr ? value->getLoc() : start;
    return m_ast.node<ast::TableColumn>(loc(start, end), ident, value);
}

// TableContent { "+" TableContent }
auto Parser::tableContentList() -> ast::List<ast::TableContent>
{
    auto content = m_ast.list<ast::TableContent>();

    do {
        content.push_back(tableContent());
    } while (accept(TokenKind::Plus));

    return content;
}

// TableInherit | TableBody
auto Parser::tableContent() -> ast::TableContent*
{
    if (m_token.is(TokenKind::Identifier)) {
        return tableInherit();
    }

    if (m_token.is(TokenKind::BracketOpen)) {
        return tableBody();
    }

    unexpected("Expected table body");
}

// Member [ "(" Expression ")" ]
auto Parser::tableInherit() -> ast::TableInherit*
{
    auto start = m_token.getLoc();
    SourceLoc end {};
    auto* mber = member();

    ast::Expression* expr {};
    if (accept(TokenKind::ParenOpen)) {
        expr = expression();
        end = m_token.getLoc();
        expect(TokenKind::ParenClose);
    } else {
        end = mber->getLoc();
    }

    return m_ast.node<ast::TableInherit>(loc(start, end), mber, expr);
}

// "[" [ TableRowList ] "]"
auto Parser::tableBody() -> ast::TableBody*
{
    auto start = m_token.getLoc();
    expect(TokenKind::BracketOpen);
    accept(TokenKind::EndOfLine);

    auto rows = m_ast.list<ast::TableRow>();
    if (m_token.isNot(TokenKind::BracketClose)) {
        rows = tableRowList();
    }

    auto end = m_token.getLoc();
    expect(TokenKind::BracketClose);

    return m_ast.node<ast::TableBody>(loc(start, end), std::move(rows));
}

// TableRow { "\n" TableRow }
auto Parser::tableRowList() -> ast::List<ast::TableRow>
{
    auto rows = m_ast.list<ast::TableRow>();

    do {
        rows.push_back(tableRow());
        if (!accept(TokenKind::EndOfLine)) {
            break;
        }
    } while (m_token.isNot(TokenKind::BracketClose));

    return rows;
}

// TableValue { "," TableValue }
auto Parser::tableRow() -> ast::TableRow*
{
    auto start = m_token.getLoc();
    auto values = m_ast.list<ast::TableValue>();

    do {
        values.push_back(tableValue());
    } while (m_token.isNot(TokenKind::EndOfLine, TokenKind::BracketClose));

    auto end = values.back()->getLoc();

    return m_ast.node<ast::TableRow>(loc(start, end), std::move(values));
}

// Literal | StructBody
auto Parser::tableValue() -> ast::TableValue*
{
    if (m_token.isValue()) {
        auto* val = literal();
        return m_ast.node<ast::TableValue>(val->getLoc(), val);
    }

    if (m_token.is(TokenKind::BraceOpen)) {
        unexpected("structs not yet implemented");
    }

    unexpected(
        "Expected a literal or a struct, got '"s
        + std::string(m_token.description()) + "'");
}

//------------------------------------------------------------------------------
// expressions
//------------------------------------------------------------------------------

// Primary { BinaryOperator Primary }
// NOLINTNEXTLINE misc-no-recursion
auto Parser::expression() -> ast::Expression*
{
    return expression(primary(), 1);
}

// ( UnaryOperator Primary ) | ( "(" expression ")" ) | TableValue
// NOLINTNEXTLINE misc-no-recursion
auto Parser::primary() -> ast::Expression*
{
    switch (m_token.getKind()) {
    case TokenKind::LogicalNot: {
        auto start = m_token.getLoc();
        next();
        auto* rhs = primary();
        return m_ast.node<ast::UnaryExpression>(loc(start, rhs->getLoc()), TokenKind::LogicalNot, rhs);
    }
    case TokenKind::ParenOpen: {
        next();
        auto* expr = expression();
        expect(TokenKind::ParenClose);
        return expr;
    }
    default:
        return tableValue();
    }
}

// NOLINTNEXTLINE misc-no-recursion
auto Parser::expression(ast::Expression* lhs, int minPrec) -> ast::Expression*
{
    while (m_token.getPrecedence() >= minPrec) {
        auto op = m_token.getKind();
        auto prec = m_token.getPrecedence();
        next();
        auto* rhs = primary();

        while (m_token.getPrecedence() > prec) { // cppcheck-suppress knownConditionTrueFalse
            rhs = expression(rhs, m_token.getPrecedence());
        }

        lhs = m_ast.node<ast::BinaryExpression>(loc(lhs->getLoc(), rhs->getLoc()), op, lhs, rhs);
    }
    return lhs;
}

//------------------------------------------------------------------------------
// Misc
//------------------------------------------------------------------------------

// IDENTIFIER { "." IDENTIFIER };
auto Parser::member() -> ast::Member*
{
    auto start = m_token.getLoc();
    SourceLoc end {};
    auto members = m_ctx->vector<std::string_view>();

    do {
        end = m_token.getLoc();
        members.push_back(consume(TokenKind::Identifier));
    } while (accept(TokenKind::Period));

    return m_ast.node<ast::Member>(loc(start, end), std::move(members));
}

// IDENTIFIER | NUMBER | STRING;
auto Parser::literal() -> ast::Literal*
{
    if (!m_token.isValue()) {
        unexpected("Expected a value");
    }
    auto type = m_token.getKind();
    auto value = m_token.getValue();
    auto loc = m_token.getLoc();
    next();
    return m_ast.node<ast::Literal>(loc, type, value);
}

//------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------

auto Parser::accept(TokenKind kind) -> bool
{
    if (m_token.isNot(kind)) {
        return false;
    }
    next();
    return true;
}

void Parser::expect(TokenKind kind)
{
    if (m_token.isNot(kind)) {
        std::stringstream msg;
        msg << "Expected '" << Token::describe(kind) << "' "
            << "got '" << m_token.description() << "' ";
        unexpected(msg.str());
    }
    next();
}

auto Parser::consume(TokenKind kind) -> std::string_view
{
    auto value = m_token.getValue();
    expect(kind);
    return value;
}

void Parser::next()
{
    m_lexer->next(m_token);
}

void Parser::unexpected(const std::string& message)
{
    (void)this;
    throw ParserException(message);
}

auto Parser::loc(SourceLoc start, SourceLoc end) -> SourceLoc
{
    return { start, end };
}

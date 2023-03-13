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
auto Parser::parse() -> ast::Content*
{
    auto start = m_token.getLoc();

    auto stmtList = m_ast.list<ast::Statement>();
    while (m_token.isNot(TokenKind::EndOfFile)) {
        stmtList.push_back(statement());
        if (!accept(TokenKind::EndOfLine)) {
            break;
        }
    }

    expect(TokenKind::EndOfFile);
    return m_ast.node<ast::Content>(makeLoc(start, m_lastLoc), std::move(stmtList));
}

// ( Import | Table )
auto Parser::statement() -> ast::Statement
{
    switch (m_token.getKind()) {
    case TokenKind::KwImport:
        return kwImport();
    case TokenKind::KwTable:
        return kwTable();
    default:
        expected("import or table"sv);
    }
}

// "import" STRING "as" IDENTIFIER
auto Parser::kwImport() -> ast::Import*
{
    auto start = m_token.getLoc();
    expect(TokenKind::KwImport);

    auto import = consume(TokenKind::String);
    expect(TokenKind::KwAs);
    auto ident = consume(TokenKind::Identifier);

    return m_ast.node<ast::Import>(makeLoc(start, m_lastLoc), import, ident);
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

    auto columns = m_ast.list<ast::TableColumn*>();
    if (accept(TokenKind::ParenOpen)) {
        columns = tableColumnList();
        expect(TokenKind::ParenClose);
    }

    expect(TokenKind::Assign);
    auto content = tableContentList();

    return m_ast.node<ast::Table>(makeLoc(start, m_lastLoc), ident, std::move(columns), std::move(content));
}

// TableColumn { "," TableColumn }
auto Parser::tableColumnList() -> ast::List<ast::TableColumn*>
{
    auto columns = m_ast.list<ast::TableColumn*>();

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

    std::optional<ast::TableValue> value {};
    if (accept(TokenKind::Assign)) {
        value = tableValue();
    }

    return m_ast.node<ast::TableColumn>(makeLoc(start, m_lastLoc), ident, value);
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
auto Parser::tableContent() -> ast::TableContent
{
    if (m_token.is(TokenKind::Identifier)) {
        return tableInherit();
    }

    if (m_token.is(TokenKind::BracketOpen)) {
        return tableBody();
    }

    expected("table inherit or body"sv);
}

// Member [ "(" Expression ")" ]
auto Parser::tableInherit() -> ast::TableInherit*
{
    auto start = m_token.getLoc();
    auto* mber = member();

    std::optional<ast::Expression> expr {};
    if (accept(TokenKind::ParenOpen)) {
        expr = expression();
        expect(TokenKind::ParenClose);
    }

    return m_ast.node<ast::TableInherit>(makeLoc(start, m_lastLoc), mber, expr);
}

// "[" [ TableRowList ] "]"
auto Parser::tableBody() -> ast::TableBody*
{
    auto start = m_token.getLoc();
    expect(TokenKind::BracketOpen);
    accept(TokenKind::EndOfLine);

    auto rows = m_ast.list<ast::TableRow*>();
    if (m_token.isNot(TokenKind::BracketClose)) {
        rows = tableRowList();
    }

    expect(TokenKind::BracketClose);

    return m_ast.node<ast::TableBody>(makeLoc(start, m_lastLoc), std::move(rows));
}

// TableRow { "\n" TableRow }
auto Parser::tableRowList() -> ast::List<ast::TableRow*>
{
    auto rows = m_ast.list<ast::TableRow*>();

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

    return m_ast.node<ast::TableRow>(makeLoc(start, m_lastLoc), std::move(values));
}

// Literal | StructBody
auto Parser::tableValue() -> ast::TableValue
{
    return literal();
}

//------------------------------------------------------------------------------
// expressions
//------------------------------------------------------------------------------

// Primary { BinaryOperator Primary }
// NOLINTNEXTLINE misc-no-recursion
auto Parser::expression() -> ast::Expression
{
    return expression(primary(), 1);
}

// ( UnaryOperator Primary ) | ( "(" expression ")" ) | TableValue
// NOLINTNEXTLINE misc-no-recursion
auto Parser::primary() -> ast::Expression
{
    switch (m_token.getKind()) {
    case TokenKind::LogicalNot: {
        auto start = m_token.getLoc();
        next();
        return m_ast.node<ast::UnaryExpression>(
            makeLoc(start, m_lastLoc),
            TokenKind::LogicalNot,
            primary());
    }
    case TokenKind::ParenOpen: {
        next();
        auto expr = expression();
        expect(TokenKind::ParenClose);
        return expr;
    }
    default:
        return literal();
    }
}

// NOLINTNEXTLINE misc-no-recursion
auto Parser::expression(ast::Expression lhs, int min) -> ast::Expression
{
    constexpr static auto getLoc = [](const auto* node) {
        return node->getLoc();
    };

    auto start = std::visit(getLoc, lhs);
    while (m_token.getPrecedence() >= min) {
        auto op = m_token.getKind();
        auto prec = m_token.getPrecedence();
        next();
        auto rhs = primary();

        while (m_token.getPrecedence() > prec) { // cppcheck-suppress knownConditionTrueFalse
            rhs = expression(rhs, m_token.getPrecedence());
        }

        auto end = std::visit(getLoc, rhs);
        lhs = m_ast.node<ast::BinaryExpression>(makeLoc(start, end), op, lhs, rhs);
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
    auto members = std::pmr::vector<std::string_view>(m_ctx->getAllocator()); // m_ctx->vector<std::string_view>();

    do {
        members.push_back(consume(TokenKind::Identifier));
    } while (accept(TokenKind::Period));

    return m_ast.node<ast::Member>(makeLoc(start, m_lastLoc), std::move(members));
}

// IDENTIFIER | NUMBER | STRING;
auto Parser::literal() -> ast::Literal*
{
    if (!m_token.isValue()) {
        expected("identifier, number or a string"sv);
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
        expected(Token::describe(kind));
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
    m_lastLoc = m_token.getLoc();
    m_lexer->next(m_token);
}

void Parser::expected(std::string_view message)
{
    const auto* source = m_lexer->getSource();
    auto pos = source->getPosition(m_token.getLoc());
    auto str = fmt::format(
        "{}({},{}): Expected {}, got {}\n"
        "{}",
        source->getName(), pos.getLine(), pos.getCol(),
        message, m_token.getString(),
        source->highlight(pos));
    throw ParserException(str);
}

auto Parser::makeLoc(SourceLoc start, SourceLoc end) -> SourceLoc
{
    return { start, end };
}

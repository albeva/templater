//
// Created by Albert on 05/03/2023.
//
#include "Parser.hpp"
#include "Ast.hpp"
#include "Lexer.hpp"
using namespace templater;
using namespace templater::table;

Parser::Parser(Lexer& lexer)
    : m_lexer { lexer }
{
    next();
}

// { Statement }
ast::Node<ast::StatementList> Parser::parse()
{
    auto start = m_token.getLoc();

    ast::List<ast::Statement> stmtList {};
    while (m_token.isNot(TokenKind::EndOfFile)) {
        stmtList.push_back(statement());
        if (!accept(TokenKind::EndOfLine)) {
            break;
        }
    }

    auto end = m_token.getLoc();
    expect(TokenKind::EndOfFile);
    return ast::make<ast::StatementList>(loc(start, end), std::move(stmtList));
}

// ( Import | Table )
ast::Node<ast::Statement> Parser::statement()
{
    ast::Node<ast::Statement> stmt {};

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
ast::Node<ast::Import> Parser::kwImport()
{
    auto start = m_token.getLoc();
    expect(TokenKind::KwImport);

    auto import = consume(TokenKind::String);
    expect(TokenKind::KwAs);
    auto end = m_token.getLoc();
    auto ident = consume(TokenKind::Identifier);

    return ast::make<ast::Import>(loc(start, end), import, ident);
}

//------------------------------------------------------------------------------
// Table
//------------------------------------------------------------------------------

// "table" IDENTIFIER [ "(" TableColumnList ")" ] [ "=" TableContentList ]
ast::Node<ast::Table> Parser::kwTable()
{
    auto start = m_token.getLoc();
    expect(TokenKind::KwTable);

    auto ident = consume(TokenKind::Identifier);

    ast::List<ast::TableColumn> columns {};
    if (accept(TokenKind::ParenOpen)) {
        columns = tableColumnList();
        expect(TokenKind::ParenClose);
    }

    expect(TokenKind::Assign);
    auto content = tableContentList();
    auto end = content.back()->loc;

    return ast::make<ast::Table>(loc(start, end), ident, std::move(columns), std::move(content));
}

// TableColumn { "," TableColumn }
ast::List<ast::TableColumn> Parser::tableColumnList()
{
    ast::List<ast::TableColumn> columns {};

    do {
        columns.push_back(tableColumn());
    } while (m_token.isNot(TokenKind::ParenClose));

    return columns;
}

// IDENTIFIER [ "=" TableValue ]
ast::Node<ast::TableColumn> Parser::tableColumn()
{
    auto start = m_token.getLoc();
    auto ident = consume(TokenKind::Identifier);
    ast::Node<ast::TableValue> value;
    if (accept(TokenKind::Assign)) {
        value = tableValue();
    }
    auto end = value ? value->loc : start;
    return ast::make<ast::TableColumn>(loc(start, end), ident, std::move(value));
}

// TableContent { "+" TableContent }
ast::List<ast::TableContent> Parser::tableContentList()
{
    ast::List<ast::TableContent> content;

    do {
        content.push_back(tableContent());
    } while (accept(TokenKind::Plus));

    return content;
}

// TableInherit | TableBody
ast::Node<ast::TableContent> Parser::tableContent()
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
ast::Node<ast::TableInherit> Parser::tableInherit()
{
    auto start = m_token.getLoc();
    SourceLoc end {};
    auto mber = member();

    ast::Node<ast::Expression> expr {};
    if (accept(TokenKind::ParenOpen)) {
        expr = expression();
        end = m_token.getLoc();
        expect(TokenKind::ParenClose);
    } else {
        end = mber->loc;
    }

    return ast::make<ast::TableInherit>(loc(start, end), std::move(mber), std::move(expr));
}

// "[" [ TableRowList ] "]"
ast::Node<ast::TableBody> Parser::tableBody()
{
    auto start = m_token.getLoc();
    expect(TokenKind::BracketOpen);
    accept(TokenKind::EndOfLine);

    ast::List<ast::TableRow> rows {};
    if (m_token.isNot(TokenKind::BracketClose)) {
        rows = tableRowList();
    }

    auto end = m_token.getLoc();
    expect(TokenKind::BracketClose);

    return ast::make<ast::TableBody>(loc(start, end), std::move(rows));
}

// TableRow { "\n" TableRow }
ast::List<ast::TableRow> Parser::tableRowList()
{
    ast::List<ast::TableRow> rows;

    do {
        rows.push_back(tableRow());
        if (!accept(TokenKind::EndOfLine)) {
            break;
        }
    } while (m_token.isNot(TokenKind::BracketClose));

    return rows;
}

// TableValue { "," TableValue }
ast::Node<ast::TableRow> Parser::tableRow()
{
    auto start = m_token.getLoc();
    ast::List<ast::TableValue> values {};

    do {
        values.push_back(tableValue());
    } while (m_token.isNot(TokenKind::EndOfLine, TokenKind::BracketClose));

    auto end = values.back()->loc;

    return ast::make<ast::TableRow>(loc(start, end), std::move(values));
}

// Literal | StructBody
ast::Node<ast::TableValue> Parser::tableValue()
{
    if (m_token.isValue()) {
        auto val = literal();
        return ast::make<ast::TableValue>(val->loc, std::move(val));
    } else if (m_token.is(TokenKind::BraceOpen)) {
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
ast::Node<ast::Expression> Parser::expression()
{
    return expression(primary(), 1);
}

// ( UnaryOperator Primary ) | ( "(" expression ")" ) | TableValue
ast::Node<ast::Expression> Parser::primary()
{
    switch (m_token.getKind()) {
    case TokenKind::LogicalNot: {
        auto start = m_token.getLoc();
        next();
        auto rhs = primary();
        return ast::make<ast::UnaryExpression>(loc(start, rhs->loc), TokenKind::LogicalNot, std::move(rhs));
    }
    case TokenKind::ParenOpen: {
        next();
        auto expr = expression();
        expect(TokenKind::ParenClose);
        return expr;
    }
    default:
        return tableValue();
    }
}

ast::Node<ast::Expression> Parser::expression(ast::Node<ast::Expression> lhs, int minPrec)
{
    while (m_token.getPrecedence() >= minPrec) {
        auto op = m_token.getKind();
        auto prec = m_token.getPrecedence();
        next();
        auto rhs = primary();

        while (m_token.getPrecedence() > prec) { // cppcheck-suppress knownConditionTrueFalse
            rhs = expression(std::move(rhs), m_token.getPrecedence());
        }

        lhs = ast::make<ast::BinaryExpression>(
            loc(lhs->loc, rhs->loc),
            op,
            std::move(lhs),
            std::move(rhs));
    }
    return lhs;
}

//------------------------------------------------------------------------------
// Misc
//------------------------------------------------------------------------------

// IDENTIFIER { "." IDENTIFIER };
ast::Node<ast::Member> Parser::member()
{
    auto start = m_token.getLoc();
    SourceLoc end {};
    std::vector<std::string_view> members {};

    do {
        end = m_token.getLoc();
        members.push_back(consume(TokenKind::Identifier));
    } while (accept(TokenKind::Period));

    return ast::make<ast::Member>(loc(start, end), std::move(members));
}

// IDENTIFIER | NUMBER | STRING;
ast::Node<ast::Literal> Parser::literal()
{
    if (!m_token.isValue()) {
        unexpected("Expected a value");
    }
    auto type = m_token.getKind();
    auto value = m_token.getValue();
    auto loc = m_token.getLoc();
    next();
    return ast::make<ast::Literal>(loc, type, value);
}

//------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------

bool Parser::accept(TokenKind kind)
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

std::string_view Parser::consume(TokenKind kind)
{
    auto value = m_token.getValue();
    expect(kind);
    return value;
}

void Parser::next()
{
    m_lexer.next(m_token);
}

void Parser::unexpected(const std::string& message)
{
    (void)this;
    throw ParserException(message);
}

SourceLoc Parser::loc(SourceLoc start, SourceLoc end)
{
    return { start, end };
}

//
// Created by Albert on 05/03/2023.
//
#include "Parser.hpp"
#include "Ast.hpp"
#include "Lexer.hpp"
using namespace templater::table;

Parser::Parser(Lexer& lexer)
    : m_lexer { lexer }
{
    next();
}

// { Statement }
ast::Node<ast::StatementList> Parser::parse()
{
    ast::List<ast::Statement> stmtList {};
    while (m_token.isNot(TokenKind::EndOfFile)) {
        stmtList.push_back(statement());
        if (!accept(TokenKind::EndOfLine)) {
            break;
        }
    }
    expect(TokenKind::EndOfFile);
    return ast::make<ast::StatementList>(std::move(stmtList));
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
    expect(TokenKind::KwImport);

    auto import = consume(TokenKind::String);
    expect(TokenKind::KwAs);
    auto ident = consume(TokenKind::Identifier);

    return ast::make<ast::Import>(import, ident);
}

//------------------------------------------------------------------------------
// Table
//------------------------------------------------------------------------------

// "table" IDENTIFIER [ "(" TableColumnList ")" ] [ "=" TableContentList ]
ast::Node<ast::Table> Parser::kwTable()
{
    expect(TokenKind::KwTable);

    auto ident = consume(TokenKind::Identifier);

    ast::List<ast::TableColumn> columns {};
    if (accept(TokenKind::ParenOpen)) {
        columns = tableColumnList();
        expect(TokenKind::ParenClose);
    }

    expect(TokenKind::Assign);
    auto content = tableContentList();

    return ast::make<ast::Table>(ident, std::move(columns), std::move(content));
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
    auto ident = consume(TokenKind::Identifier);
    ast::Node<ast::TableValue> value;
    if (accept(TokenKind::Assign)) {
        value = tableValue();
    }
    return ast::make<ast::TableColumn>(ident, std::move(value));
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
    auto mber = member();

    ast::Node<ast::Expression> expr {};
    if (accept(TokenKind::ParenOpen)) {
        expr = expression();
        expect(TokenKind::ParenClose);
    }

    return ast::make<ast::TableInherit>(std::move(mber), std::move(expr));
}

// "[" [ TableRowList ] "]"
ast::Node<ast::TableBody> Parser::tableBody()
{
    expect(TokenKind::BracketOpen);
    accept(TokenKind::EndOfLine);

    ast::List<ast::TableRow> rows {};
    if (m_token.isNot(TokenKind::BracketClose)) {
        rows = tableRowList();
    }
    expect(TokenKind::BracketClose);

    return ast::make<ast::TableBody>(std::move(rows));
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
    ast::List<ast::TableValue> values {};

    do {
        values.push_back(tableValue());
    } while (m_token.isNot(TokenKind::EndOfLine, TokenKind::BracketClose));

    return ast::make<ast::TableRow>(std::move(values));
}

// Literal | StructBody
ast::Node<ast::TableValue> Parser::tableValue()
{
    if (m_token.isValue()) {
        return ast::make<ast::TableValue>(literal());
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
    case TokenKind::LogicalNot:
        next();
        return ast::make<ast::UnaryExpression>(TokenKind::LogicalNot, primary());
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

        lhs = ast::make<ast::BinaryExpression>(op, std::move(lhs), std::move(rhs));
    }
    return lhs;
}

//------------------------------------------------------------------------------
// Misc
//------------------------------------------------------------------------------

// IDENTIFIER { "." IDENTIFIER };
ast::Node<ast::Member> Parser::member()
{
    std::vector<std::string_view> members {};

    do {
        members.push_back(consume(TokenKind::Identifier));
    } while (accept(TokenKind::Period));

    return ast::make<ast::Member>(std::move(members));
}

// IDENTIFIER | NUMBER | STRING;
ast::Node<ast::Literal> Parser::literal()
{
    if (!m_token.isValue()) {
        unexpected("Expected a value");
    }
    auto type = m_token.getKind();
    auto value = m_token.getValue();
    next();
    return ast::make<ast::Literal>(type, value);
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

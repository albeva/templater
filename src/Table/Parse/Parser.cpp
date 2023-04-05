//
// Created by Albert on 05/03/2023.
//
#include "Parser.hpp"
#include "Lexer.hpp"
#include "Support/Diagnostics.hpp"
#include "Table/Ast/Context.hpp"
using support::Diagnostics;
using support::SourceLoc;
using support::Visitor;
using table::Identifier;
using table::StringLiteral;
using table::parser::Parser;
using namespace std::literals;

Parser::Parser(Diagnostics* diag, Lexer* lexer)
    : m_diag(diag)
    , m_lexer(lexer)
    , m_ast(nullptr)
{
    next();
}

// { Statement }
auto Parser::parse() -> std::unique_ptr<ast::Context>
{
    m_ast = std::make_unique<ast::Context>(m_lexer->getSource());

    auto start = m_token.getLoc();
    auto stmtList = m_ast->list<ast::Statement>();
    while (m_token.isNot(TokenKind::EndOfFile)) {
        stmtList.push_back(statement());
        if (!accept(TokenKind::EndOfLine)) {
            break;
        }
    }

    expect(TokenKind::EndOfFile);
    auto* node = m_ast->node<ast::Content>(makeLoc(start, m_lastLoc), std::move(stmtList));
    m_ast->setRoot(node);

    return std::move(m_ast);
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

    auto file = stringLiteral();
    expect(TokenKind::KwAs);
    auto ident = identifier();

    return m_ast->node<ast::Import>(makeLoc(start, m_lastLoc), ident, file);
}

//------------------------------------------------------------------------------
// Table
//------------------------------------------------------------------------------

// "table" IDENTIFIER [ "(" TableColumnList ")" ] [ "=" TableContentList ]
auto Parser::kwTable() -> ast::Table*
{
    auto start = m_token.getLoc();
    expect(TokenKind::KwTable);

    auto ident = identifier();

    auto columns = m_ast->list<ast::TableColumn*>();
    if (accept(TokenKind::ParenOpen)) {
        columns = tableColumnList();
        expect(TokenKind::ParenClose);
    }

    expect(TokenKind::Assign);
    auto content = tableContentList();

    return m_ast->node<ast::Table>(makeLoc(start, m_lastLoc), ident, std::move(columns), std::move(content));
}

// TableColumn { "," TableColumn }
auto Parser::tableColumnList() -> ast::List<ast::TableColumn*>
{
    auto columns = m_ast->list<ast::TableColumn*>();

    do {
        columns.push_back(tableColumn());
    } while (m_token.isNot(TokenKind::ParenClose));

    return columns;
}

// IDENTIFIER [ "=" TableValue ]
auto Parser::tableColumn() -> ast::TableColumn*
{
    auto start = m_token.getLoc();
    auto ident = identifier();

    std::optional<Value> val {};
    if (accept(TokenKind::Assign)) {
        val = value();
    }

    return m_ast->node<ast::TableColumn>(makeLoc(start, m_lastLoc), ident, val);
}

// TableContent { "+" TableContent }
auto Parser::tableContentList() -> ast::List<ast::TableContent>
{
    auto content = m_ast->list<ast::TableContent>();

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

    expected("["sv);
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

    return m_ast->node<ast::TableInherit>(makeLoc(start, m_lastLoc), mber, expr);
}

// "[" [ TableRowList ] "]"
auto Parser::tableBody() -> ast::TableBody*
{
    auto start = m_token.getLoc();
    expect(TokenKind::BracketOpen);
    accept(TokenKind::EndOfLine);

    auto rows = m_ast->list<ast::TableRow*>();
    if (m_token.isNot(TokenKind::BracketClose)) {
        rows = tableRowList();
    }

    expect(TokenKind::BracketClose);

    return m_ast->node<ast::TableBody>(makeLoc(start, m_lastLoc), std::move(rows));
}

// TableRow { "\n" TableRow }
auto Parser::tableRowList() -> ast::List<ast::TableRow*>
{
    auto rows = m_ast->list<ast::TableRow*>();

    do {
        rows.push_back(tableRow());
        if (!accept(TokenKind::EndOfLine)) {
            break;
        }
    } while (m_token.isNot(TokenKind::BracketClose));

    return rows;
}

// Value { "," Value }
auto Parser::tableRow() -> ast::TableRow*
{
    auto start = m_token.getLoc();
    auto values = m_ast->list<ast::TableValue>();

    do {
        if (accept(TokenKind::Minus)) {
            values.emplace_back();
        } else if (accept(TokenKind::Pipe)) {
            values.emplace_back(ast::PipeLiteral {});
        } else {
            values.emplace_back(value());
        }
    } while (m_token.isNot(TokenKind::EndOfLine, TokenKind::BracketClose));

    return m_ast->node<ast::TableRow>(makeLoc(start, m_lastLoc), std::move(values));
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
        auto op = operation();
        return m_ast->node<ast::UnaryExpression>(
            makeLoc(op.getLoc(), m_lastLoc),
            op,
            primary());
    }
    case TokenKind::ParenOpen: {
        next();
        auto expr = expression();
        expect(TokenKind::ParenClose);
        return expr;
    }
    case TokenKind::Identifier:
        return member();
    case TokenKind::Number:
        return numberLiteral();
    case TokenKind::String:
        return stringLiteral();
    default:
        expected("expression");
    }
}

// NOLINTNEXTLINE misc-no-recursion
auto Parser::expression(ast::Expression lhs, int min) -> ast::Expression
{
    constexpr static auto getLoc = Visitor {
        [](const Identifier& val) {
            return val.getLoc();
        },
        [](const NumberLiteral& val) {
            return val.getLoc();
        },
        [](const StringLiteral& val) {
            return val.getLoc();
        },
        []<typename T>(const T* val) {
            return val->getLoc();
        },
    };

    auto start = std::visit(getLoc, lhs);
    while (m_token.getPrecedence() >= min) {
        auto prec = m_token.getPrecedence();
        auto op = operation();
        auto rhs = primary();

        while (m_token.getPrecedence() > prec) { // cppcheck-suppress knownConditionTrueFalse
            rhs = expression(rhs, m_token.getPrecedence());
        }

        auto end = std::visit(getLoc, rhs);
        lhs = m_ast->node<ast::BinaryExpression>(makeLoc(start, end), op, lhs, rhs);
    }
    return lhs;
}

auto Parser::operation() -> ast::Operation
{
    auto op = m_token;
    if (op.getPrecedence() == 0) {
        expected("operator");
    }
    next();
    return { op.getLoc(), op.getKind() };
}

//------------------------------------------------------------------------------
// Misc
//------------------------------------------------------------------------------

// IDENTIFIER { "." IDENTIFIER };
auto Parser::member() -> ast::Member*
{
    auto start = m_token.getLoc();
    auto members = m_ast->list<Identifier>();

    do {
        members.emplace_back(identifier());
    } while (accept(TokenKind::Period));

    return m_ast->node<ast::Member>(makeLoc(start, m_lastLoc), std::move(members));
}

// IDENTIFIER
auto Parser::identifier() -> Identifier
{
    auto ident = m_token;
    expect(TokenKind::Identifier);
    return { ident.getLoc(), ident.getValue() };
}

// STRING
auto Parser::stringLiteral() -> StringLiteral
{
    auto str = m_token;
    expect(TokenKind::String);
    return { str.getLoc(), str.getValue() };
}

// NUMBER
auto Parser::numberLiteral() -> NumberLiteral
{
    auto num = m_token;

    unsigned val {};
    const std::string_view& str = num.getValue();
    if (std::from_chars(str.data(), str.data() + str.size(), val).ec == std::errc {}) {
        expect(TokenKind::Number);
        return { num.getLoc(), val };
    }
    expected("number");
}

auto Parser::value() -> Value
{
    switch (m_token.getKind()) {
    case TokenKind::Identifier:
        return identifier();
    case TokenKind::String:
        return stringLiteral();
    case TokenKind::Number:
        return numberLiteral();
    default:
        expected("value");
    }
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

void Parser::next()
{
    m_lastLoc = m_token.getLoc();
    m_lexer->next(m_token);
}

void Parser::expected(std::string_view message)
{
    m_diag->error(
        m_lexer->getSource(), m_token.getLoc(),
        fmt::format("Expected {}, got {}", message, m_token.getString()));
    throw support::EmptyException();
}

auto Parser::makeLoc(SourceLoc start, SourceLoc end) noexcept -> SourceLoc
{
    return { start, end };
}

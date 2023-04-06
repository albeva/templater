//
// Created by Albert on 19/03/2023.
//
#include "Templater.hpp"
#include "Lexer.hpp"
#include "Support/GlobalContext.hpp"
#include "Support/Diagnostics.hpp"
#include "Support/Source.hpp"
#include "Token.hpp"
using tpl::Templater;

Templater::Templater(support::GlobalContext* ctx, support::Source* source, table::SymbolTable* symbolTable)
    : m_ctx(ctx)
    , m_source(source)
    , m_symbolTable(symbolTable)
    , m_current{0}
{
    (void)m_symbolTable;
    collectTokens();
    parse();
}

Templater::~Templater() = default;

void Templater::collectTokens()
{
    Lexer lexer { m_ctx, m_source->buffer() };
    do {
        lexer.next(m_tokens.emplace_back());
        if (m_tokens.back().is(TokenKind::Invalid)) {
            break;
        }
    } while (m_tokens.back().isNot(TokenKind::EndOfFile));
}

void Templater::parse()
{
    while(true) {
        const auto& token = current();
        switch (token.getKind()) {
        case TokenKind::Text:
            m_output << token.getValue();
            ++m_current;
            break;
        case TokenKind::EndOfFile:
            return;
        case TokenKind::ExpressionOpen:
            expression();
            break;
        case TokenKind::StatementOpen:
            statement();
            break;
        default:
            expected("text, statement or an expression");
        }
    }
}

void Templater::expression()
{
    ++m_current;
    (void)this;
}

void Templater::statement()
{
    ++m_current;
    (void)this;
}

//------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------

auto Templater::accept(TokenKind kind) -> bool
{
    if (current().isNot(kind)) {
        return false;
    }
    ++m_current;
    return true;
}

void Templater::expect(TokenKind kind)
{
    if (current().isNot(kind)) {
        expected(Token::describe(kind));
    }
    ++m_current;
}

void Templater::expected(std::string_view message)
{
    const auto& token = current();
    m_ctx->getDiagnostics()->error(
        m_source, token.getLoc(),
        fmt::format("Expected {}, got {}", message, token.getString()));
    throw support::DiagException();
}
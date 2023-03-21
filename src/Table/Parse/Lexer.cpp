//
// Created by Albert on 05/03/2023.
//
#include "Lexer.hpp"
#include "Support/GlobalContext.hpp"
#include "Support/Source.hpp"
#include "Token.hpp"
using table::parser::Lexer;
using table::parser::Token;
using table::parser::TokenKind;
using namespace std::literals;

Lexer::Lexer(support::GlobalContext* ctx, support::Source* source) noexcept
    : LexerBase(ctx, source->source())
    , m_source(source)
{
}

void Lexer::next(Token& token) // NOLINT readability-function-cognitive-complexity
{
    SCOPE_GAURD({
        if (token.is(TokenKind::EndOfLine)) {
            m_hasStmt = false;
        } else if (token.is(TokenKind::EndOfFile) && m_hasStmt) {
            m_hasStmt = false;
            token.set(TokenKind::EndOfLine, token.getLoc(), token.getValue());
        } else if (token.isNot(TokenKind::Invalid)) {
            m_hasStmt = true;
        }
    });

    std::size_t size = 1;
    using enum TokenKind;
    while (isValid()) {
        auto ch = current();
        switch (ch) {
        case ' ':
        case '\t':
            advance();
            continue;
        case '\r':
            if (peek() == '\n') {
                size++;
            }
            [[fallthrough]];
        case '\n':
            if (m_hasStmt) {
                return make(token, EndOfLine, size);
            }
            advance(size);
            size = 1;
            continue;
        case '#':
            advance();
            skipToLineEnd();
            continue;
        case '_':
            if (isIdentChar(peek())) {
                return identifier(token);
            }
            advance();
            skipToNextLine();
            continue;
        case '"':
            return string(token);
        case '(':
            return make(token, ParenOpen);
        case ')':
            return make(token, ParenClose);
        case '[':
            return make(token, BracketOpen);
        case ']':
            return make(token, BracketClose);
        case '{':
            return make(token, BraceOpen);
        case '}':
            return make(token, BraceClose);
        case '<':
            if (peek() == '=') {
                return make(token, LessOrEqual, 2);
            }
            return make(token, Less, 2);
        case '>':
            if (peek() == '=') {
                return make(token, GreaterOrEqual, 2);
            }
            return make(token, Greater, 2);
        case '=':
            if (peek() == '=') {
                return make(token, Equal, 2);
            }
            return make(token, Assign);
        case '.':
            return make(token, Period);
        case '+':
            return make(token, Plus);
        case '-':
            return make(token, Minus);
        case '!':
            if (peek() == '=') {
                return make(token, NotEqual, 2);
            }
            return make(token, LogicalNot);
        case '&':
            if (peek() == '&') {
                return make(token, LogicalAnd, 2);
            }
            return unexpected(token, "'&'. Did you mean '&&'?");
        case '|':
            if (peek() == '|') {
                return make(token, LogicalOr, 2);
            }
            return make(token, Pipe);
        default:
            if (isAlpha(ch)) {
                return identifier(token);
            }
            if (isDigit(ch)) {
                return number(token);
            }
            return unexpected(token);
        }
    }
    // no more input
    return make(token, EndOfFile, 0);
}

void Lexer::skipToLineEnd() noexcept
{
    while (isValid() && !isLineEnd(current())) {
        advance();
    }
}

void Lexer::skipToNextLine() noexcept
{
    skipToLineEnd();
    if (isValid() && current() == '\r') {
        advance();
    }
    if (isValid() && current() == '\n') {
        advance();
    }
}

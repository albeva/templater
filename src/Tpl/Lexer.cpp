//
// Created by Albert on 19/03/2023.
//
#include "Lexer.hpp"
using tpl::Lexer;
using namespace std::literals;

Lexer::Lexer(support::GlobalContext* ctx, std::string_view buffer) noexcept
    : LexerBase(ctx, buffer)
    , m_state(State::Text)
{
}

void Lexer::next(Token& token)
{
    switch (m_state) {
    case State::Text:
        return text(token);
    case State::Statement:
    case State::Expression:
        return grammar(token);
    default:
        std::unreachable();
    }
}

void Lexer::text(Token& token)
{
    auto start = getInput();
    while (isValid()) {
        auto ch = current();
        if (ch == '{' && peek() == '%') {
            if (start != getInput()) {
                goto makeText; // NOLINT
            }
            advance(2);
            m_state = State::Statement;
            return make(token, Kind::StatementOpen, 2);
        }
        if (ch == '$' && peek() == '{') {
            if (start != getInput()) {
                goto makeText; // NOLINT
            }
            advance(2);
            m_state = State::Expression;
            return make(token, Kind::ExpressionOpen, 2);
        }
        // FIXME: Handle escape sequence "\${" and "\${"
        advance();
    }

    if (start == getInput()) {
        return make(token, TokenKind::EndOfFile, 0);
    }

makeText:
    return token.set(Kind::Text, loc(start), lexeme(start, getInput()));
}

void Lexer::grammar(Token& token)
{
    while (isValid()) {
        auto ch = current();
        switch (ch) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            advance();
            continue;
        case '|':
            return make(token, Kind::Pipe);
        case '.':
            return make(token, Kind::Period);
        case '[':
            return make(token, Kind::BracketOpen);
        case ']':
            return make(token, Kind::BracketClose);
        case '(':
            return make(token, Kind::ParenOpen);
        case ')':
            return make(token, Kind::ParenClose);
        case '"':
            return string(token);
        case '%':
            if (m_state == State::Statement) {
                if (peek() == '}') {
                    m_state = State::Text;
                    return make(token, TokenKind::StatementClose, 2);
                }
                return unexpected(token, "did you mean '%}'?"sv);
            }
            return unexpected(token, "did you mean '}'?"sv);
        case '}':
            if (m_state == State::Expression) {
                m_state = State::Text;
                return make(token, Kind::ExpressionClose);
            }
            return unexpected(token, "did you mean '%}'?"sv);
        default:
            if (isDigit(ch)) {
                return number(token);
            }
            if (isAlpha(ch) || ch == '_') {
                return identifier(token);
            }
            return unexpected(token);
        }
    }
    return make(token, TokenKind::EndOfFile, 0);
}

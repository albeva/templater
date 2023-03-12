//
// Created by Albert on 05/03/2023.
//
#include "Lexer.hpp"
#include "Support/Source.hpp"
#include "Token.hpp"
using namespace templater::table;

namespace {
[[nodiscard]] inline bool isDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

[[nodiscard]] inline bool isAlpha(char ch)
{
    return (ch >= 'a' && ch <= 'z')
        || (ch >= 'A' && ch <= 'Z');
}

[[nodiscard]] inline bool isIdentChar(char ch)
{
    return isAlpha(ch) || isDigit(ch) || ch == '_';
}

[[nodiscard]] inline bool isLineOrFileEnd(char ch)
{
    return ch == '\n' || ch == '\r' || ch == '\0';
}

using Keyword = std::pair<std::string_view, TokenKind>;
constexpr std::array kKeywords {
    Keyword { "as", TokenKind::KwAs },
    Keyword { "by", TokenKind::KwBy },
    Keyword { "import", TokenKind::KwImport },
    Keyword { "order", TokenKind::KwOrder },
    Keyword { "table", TokenKind::KwTable },
};
} // namespace

Lexer::Lexer(Source& source)
    : m_input { source.data() }
{
}

void Lexer::next(Token& token)
{
    std::size_t size = 1;

    while (true) {
        auto ch = *m_input;
        switch (ch) {
        case '\0':
            return make(token, TokenKind::EndOfFile, 0);
        case ' ':
        case '\t':
            m_input++;
            continue;
        case '\r':
            if (m_input[1] == '\n') {
                size++;
            }
            // fall through
        case '\n':
            if (m_hasStmt) {
                return make(token, TokenKind::EndOfLine, size);
            }
            std::advance(m_input, size);
            size = 1;
            continue;
        case '#':
            m_input++;
            skipToLineEnd();
            continue;
        case '_':
            if (isIdentChar(m_input[1])) {
                return identifier(token);
            }
            m_input++;
            skipToNextLine();
            continue;
        case '"':
            return string(token);
        case '(':
            return make(token, TokenKind::ParenOpen);
        case ')':
            return make(token, TokenKind::ParenClose);
        case '[':
            return make(token, TokenKind::BracketOpen);
        case ']':
            return make(token, TokenKind::BracketClose);
        case '{':
            return make(token, TokenKind::BraceOpen);
        case '}':
            return make(token, TokenKind::BraceClose);
        case '=':
            if (m_input[1] == '=') {
                return make(token, TokenKind::Equal, 2);
            }
            return make(token, TokenKind::Assign);
        case '.':
            return make(token, TokenKind::Period);
        case '+':
            return make(token, TokenKind::Plus);
        case '!':
            if (m_input[1] == '=') {
                return make(token, TokenKind::NotEqual, 2);
            }
            return make(token, TokenKind::LogicalNot);
        case '&':
            if (m_input[1] == '&') {
                return make(token, TokenKind::LogicalAnd, 2);
            }
            return unexpected(token, "'&'. Did you mean '&&'?");
        case '|':
            if (m_input[1] == '|') {
                return make(token, TokenKind::LogicalOr, 2);
            }
            return unexpected(token, "'|'. Did you mean '||'?");
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
}

void Lexer::skipToLineEnd()
{
    while (!isLineOrFileEnd(*m_input)) {
        m_input++;
    }
}

void Lexer::skipToNextLine()
{
    skipToLineEnd();
    if (*m_input == '\r') {
        m_input++;
    }
    if (*m_input == '\n') {
        m_input++;
    }
}

void Lexer::make(Token& token, TokenKind kind, std::size_t len)
{
    m_hasStmt = kind != TokenKind::EndOfLine;
    const auto* start = m_input;
    std::advance(m_input, len);
    token.set(kind, loc(start));
}

void Lexer::unexpected(Token& token, std::string_view message)
{
    const auto* start = m_input;
    if (*m_input != '\0') {
        m_input++;
    }
    if (message.empty()) {
        message = {start, m_input};
    }
    token.set(TokenKind::Invalid, loc(start), message);
}

void Lexer::string(Token& token)
{
    m_hasStmt = true;

    const auto* start = m_input++;
    while (true) {
        auto ch = *m_input;
        if (isLineOrFileEnd(ch)) {
            return unexpected(token, "end of line");
        }
        m_input++;

        if (ch == '\"') {
            break;
        }
    }

    token.set(TokenKind::String, loc(start), { start + 1, m_input - 1 });
}

void Lexer::number(Token& token)
{
    m_hasStmt = true;

    const auto* start = m_input++;
    while (isDigit(*m_input)) {
        m_input++;
    }

    token.set(TokenKind::Number, loc(start), { start, m_input });
}

void Lexer::identifier(Token& token)
{
    m_hasStmt = true;

    const auto* start = m_input++;
    while (isIdentChar(*m_input)) {
        m_input++;
    }

    auto lexeme = std::string_view { start, m_input };
    if (const auto& iter = ranges::find(kKeywords, lexeme, &Keyword::first); iter != kKeywords.end()) {
        token.set(iter->second, loc(start));
    } else {
        token.set(TokenKind::Identifier, loc(start), lexeme);
    }
}

templater::SourceLoc Lexer::loc(const char* start)
{
    return { .start = start, .end = m_input };
}

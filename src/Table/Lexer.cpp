//
// Created by Albert on 05/03/2023.
//
#include "Lexer.hpp"
#include "Support/Context.hpp"
#include "Support/Source.hpp"
#include "Token.hpp"
using namespace templater::table;

// Lexer iterates over null terminated const char* using pointer
// arithmetic
// !NOLINTBEGIN cppcoreguidelines-pro-bounds-pointer-arithmetic

namespace {
[[nodiscard]] inline auto isDigit(char ch) -> bool
{
    return ch >= '0' && ch <= '9';
}

[[nodiscard]] inline auto isAlpha(char ch) -> bool
{
    return (ch >= 'a' && ch <= 'z')
        || (ch >= 'A' && ch <= 'Z');
}

[[nodiscard]] inline auto isIdentChar(char ch) -> bool
{
    return isAlpha(ch) || isDigit(ch) || ch == '_';
}

[[nodiscard]] inline auto isLineOrFileEnd(char ch) -> bool
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

Lexer::Lexer(Context* ctx, Source* source)
    : m_ctx(ctx)
    , m_source(source)
    , m_input(source->data())
{
}

void Lexer::next(Token& token) // NOLINT readability-function-cognitive-complexity
{
    std::size_t size = 1;

    while (true) {
        auto ch = *m_input;
        switch (ch) {
        case '\0':
            if (m_hasStmt) {
                return make(token, TokenKind::EndOfLine, 0);
            }
            return make(token, TokenKind::EndOfFile, 0);
        case ' ':
        case '\t':
            m_input++;
            continue;
        case '\r':
            if (m_input[1] == '\n') {
                size++;
            }
            [[fallthrough]];
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
        case '<':
            if (m_input[1] == '=') {
                return make(token, TokenKind::LessOrEqual, 2);
            }
            return make(token, TokenKind::Less, 2);
        case '>':
            if (m_input[1] == '=') {
                return make(token, TokenKind::GreaterOrEqual, 2);
            }
            return make(token, TokenKind::Greater, 2);
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
        message = { start, m_input };
    }
    token.set(TokenKind::Invalid, loc(start), message);
}

void Lexer::string(Token& token)
{
    const auto* start = m_input++;
    const auto* begin = m_input;

    std::string literal {};
    while (true) {
        auto ch = *m_input;
        switch (ch) {
        case '\0':
        case '\n':
        case '\r':
            return unexpected(token, "open string");
        case '\\': {
            if (begin < m_input) {
                literal.append(begin, m_input);
            }
            auto esc = escape();
            if (esc.has_value()) {
                literal += esc.value();
            } else {
                return unexpected(token, esc.error());
            }
            m_input++;
            begin = m_input;
            continue;
        }
        case '"':
            break;
        default:
            m_input++;
            continue;
        }
        break;
    }

    auto lit = [&]() {
        if (literal.empty()) {
            return m_ctx->retain(std::string_view { begin, m_input });
        }
        if (begin < m_input) {
            literal.append(begin, m_input);
        }
        return m_ctx->retain(std::move(literal));
    }();

    m_input++;
    m_hasStmt = true;
    token.set(TokenKind::String, loc(start), lit);
}

auto Lexer::escape() -> std::expected<char, std::string_view>
{
    // assume m_input[0] == '\\'
    switch (*++m_input) {
    case 'a':
        return '\a';
    case 'b':
        return '\b';
    case 'f':
        return '\f';
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    case 'v':
        return '\v';
    case '\\':
        return '\\';
    case '\'':
        return '\'';
    case '"':
        return '"';
    default:
        return std::unexpected("invalid escape sequence"sv);
    }
}

void Lexer::number(Token& token)
{
    const auto* start = m_input++;
    while (isDigit(*m_input)) {
        m_input++;
    }

    m_hasStmt = true;
    token.set(TokenKind::Number, loc(start), m_ctx->retain(std::string_view { start, m_input }));
}

void Lexer::identifier(Token& token)
{
    const auto* start = m_input++;
    while (isIdentChar(*m_input)) {
        m_input++;
    }

    m_hasStmt = true;
    auto lexeme = std::string_view { start, m_input };
    if (const auto& iter = std::ranges::find(kKeywords, lexeme, &Keyword::first); iter != kKeywords.end()) {
        token.set(iter->second, loc(start));
    } else {
        token.set(TokenKind::Identifier, loc(start), m_ctx->retain(lexeme));
    }
}

auto Lexer::loc(const char* start) -> templater::SourceLoc
{
    return {
        static_cast<unsigned>(std::distance(m_source->data(), start)),
        static_cast<unsigned>(std::distance(m_source->data(), m_input))
    };
}

// NOLINTEND cppcoreguidelines-pro-bounds-pointer-arithmetic

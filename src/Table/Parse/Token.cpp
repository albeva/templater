//
// Created by Albert on 05/03/2023.
//
#include "Token.hpp"
using table::parser::Token;
using table::parser::TokenInformation;
using namespace std::literals;

auto TokenInformation::describe(Kind kind) noexcept -> std::string_view
{
    using p = std::pair<Kind, std::string_view>;
    constexpr static std::array tokens {
        p { Kind::Invalid, "<Invalid>"sv },
        p { Kind::EndOfFile, "<End-Of-File>"sv },
        p { Kind::EndOfLine, "<End-Of-Line>"sv },
        p { Kind::String, "<String-Literal>"sv },
        p { Kind::Number, "<Number-Literal>"sv },
        p { Kind::Identifier, "<Identifier>"sv },
        p { Kind::ParenOpen, "("sv },
        p { Kind::ParenClose, ")"sv },
        p { Kind::BraceOpen, "{"sv },
        p { Kind::BraceClose, "}"sv },
        p { Kind::BracketOpen, "["sv },
        p { Kind::BracketClose, "]"sv },
        p { Kind::Assign, "="sv },
        p { Kind::Period, "."sv },
        p { Kind::Plus, "+"sv },
        p { Kind::Minus, "-"sv },
        p { Kind::Pipe, "|"sv },
        p { Kind::Less, "<"sv },
        p { Kind::LessOrEqual, "<="sv },
        p { Kind::GreaterOrEqual, ">="sv },
        p { Kind::Greater, ">"sv },
        p { Kind::Equal, "=="sv },
        p { Kind::NotEqual, "!="sv },
        p { Kind::LogicalAnd, "&&"sv },
        p { Kind::LogicalOr, "||"sv },
        p { Kind::LogicalNot, "!"sv },
        p { Kind::KwTable, "table"sv },
        p { Kind::KwImport, "import"sv },
        p { Kind::KwAs, "as"sv },
    };

    if (auto iter = std::ranges::find(tokens, kind, &p::first); iter != tokens.end()) {
        return iter->second;
    }
    std::unreachable();
}

auto TokenInformation::getKind(std::string_view id) noexcept -> TokenInformation::Kind
{
    using p = std::pair<std::string_view, TokenKind>;
    constexpr std::array keywords {
        p { "as", TokenKind::KwAs },
        p { "import", TokenKind::KwImport },
        p { "table", TokenKind::KwTable },
    };

    if (auto iter = std::ranges::find(keywords, id, &p::first); iter != keywords.end()) {
        return iter->second;
    }

    return TokenKind::Identifier;
}

auto Token::getString() const noexcept -> std::string_view
{
    if (isValue() || is(TokenKind::Invalid)) {
        return getValue();
    }
    return description();
}

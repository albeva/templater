//
// Created by Albert on 20/03/2023.
//
#include "Token.hpp"
using tpl::Token;
using tpl::TokenInformation;
using namespace std::literals;

auto TokenInformation::describe(Kind kind) noexcept -> std::string_view
{
    using p = std::pair<Kind, std::string_view>;
    static constexpr std::array tokens = {
        p { Kind::Invalid, "<Invalid>"sv },
        p { Kind::EndOfFile, "<End-Of-File>"sv },
        p { Kind::Text, "<Text>"sv },
        p { Kind::String, "<String-Literal>"sv },
        p { Kind::Number, "<Number-Literal>"sv },
        p { Kind::Identifier, "<Identifier>"sv },
        p { Kind::StatementOpen, "%{"sv },
        p { Kind::StatementClose, "}%"sv },
        p { Kind::ExpressionOpen, "${"sv },
        p { Kind::ExpressionClose, "}"sv },
        p { Kind::Pipe, "|"sv },
        p { Kind::Period, "."sv },
        p { Kind::ParenOpen, "("sv },
        p { Kind::ParenClose, ")"sv },
        p { Kind::BracketOpen, "["sv },
        p { Kind::BracketClose, "]"sv },
        p { Kind::KwFor, "for"sv },
        p { Kind::KwIn, "in"sv },
        p { Kind::KwEndFor, "endfor"sv },
        p { Kind::KwIf, "if"sv },
        p { Kind::KwElseIf, "elseif"sv },
        p { Kind::KwElse, "else"sv },
        p { Kind::KwEndIf, "endif"sv },
    };

    if (auto iter = std::ranges::find(tokens, kind, &p::first); iter != tokens.end()) {
        return iter->second;
    }
    std::unreachable();
}

auto TokenInformation::getKind(std::string_view id) noexcept -> Kind
{
    using p = std::pair<std::string_view, Kind>;
    constexpr static std::array keywords {
        p { "else", Kind::KwElse },
        p { "elseif", Kind::KwElseIf },
        p { "endfor", Kind::KwEndFor },
        p { "endif", Kind::KwEndIf },
        p { "for", Kind::KwFor },
        p { "if", Kind::KwIf },
        p { "in", Kind::KwIn },
    };

    if (auto iter = std::ranges::find(keywords, id, &p::first); iter != keywords.end()) {
        return iter->second;
    }
    return Kind::Identifier;
}

auto Token::getString() const noexcept -> std::string_view
{
    if (isValue() || is(TokenKind::Invalid)) {
        return getValue();
    }
    return describe(getKind());
}
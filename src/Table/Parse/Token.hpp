//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/SourceLoc.hpp"
#include "Support/TokenBase.hpp"
namespace table::parser {

enum class TokenKind {
    Invalid,
    EndOfFile,
    EndOfLine,
    String,
    Number,
    Identifier,
    ParenOpen,
    ParenClose,
    BraceOpen,
    BraceClose,
    BracketOpen,
    BracketClose,
    Assign,
    Period,
    Plus,
    Minus,
    Pipe,
    Less,
    LessOrEqual,
    GreaterOrEqual,
    Greater,
    Equal,
    NotEqual,
    LogicalAnd,
    LogicalOr,
    LogicalNot,
    KwTable,
    KwImport,
    KwAs,
};

struct TokenInformation final {
    using Kind = TokenKind;
    static auto describe(Kind kind) noexcept -> std::string_view;
    static auto getKind(std::string_view id) noexcept -> Kind;
};

struct Token final : support::TokenBase<TokenInformation> {
    using TokenBase::TokenBase;

    [[nodiscard]] inline auto isValue() const noexcept -> bool
    {
        return is(TokenKind::Number, TokenKind::Identifier, TokenKind::String);
    }

    [[nodiscard]] inline auto getPrecedence() const noexcept -> int
    {
        switch (getKind()) {
        case TokenKind::LogicalNot:
            return 6; // NOLINT
        case TokenKind::Less:
        case TokenKind::LessOrEqual:
        case TokenKind::GreaterOrEqual:
        case TokenKind::Greater:
            return 5; // NOLINT
        case TokenKind::Equal:
            return 4;
        case TokenKind::NotEqual:
            return 3;
        case TokenKind::LogicalAnd:
            return 2;
        case TokenKind::LogicalOr:
            return 1;
        default:
            return 0;
        }
    }

    [[nodiscard]] auto getString() const noexcept -> std::string_view;
};

} // namespace table::parser

//
// Created by Albert on 20/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/TokenBase.hpp"

namespace tpl {

enum class TokenKind {
    Invalid,
    EndOfFile,
    Text,
    Identifier,
    String,
    Number,
    StatementOpen,   // {%
    StatementClose,  // %}
    ExpressionOpen,  // ${
    ExpressionClose, // }
    Pipe,            // |
    Period,          // .
    ParenOpen,       // (
    ParenClose,      // )
    BracketOpen,     // [
    BracketClose,    // ]
    KwFor,           // for
    KwIn,            // in
    KwEndFor,        // endfor
    KwIf,            // if
    KwElseIf,        // elseif
    KwElse,          // else
    KwEndIf,         // endif
};

struct TokenInformation final {
    using Kind = TokenKind;
    static auto describe(Kind kind) noexcept -> std::string_view;
    static auto getKind(std::string_view id) noexcept -> Kind;
};

struct Token : support::TokenBase<TokenInformation> {
    using TokenBase::TokenBase;

    [[nodiscard]] inline auto isValue() const noexcept
    {
        return is(TokenKind::Identifier, TokenKind::String, TokenKind::Number);
    }

    [[nodiscard]] auto getString() const noexcept -> std::string_view;
};

} // namespace tpl

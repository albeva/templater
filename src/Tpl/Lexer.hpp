//
// Created by Albert on 19/03/2023.
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

struct Token : support::TokenBase<TokenKind> {
};

class Lexer final {
};

} // namespace tpl
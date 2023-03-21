//
// Created by Albert on 19/03/2023.
//
#include "Lexer.hpp"
using tpl::Lexer;

Lexer::Lexer(support::GlobalContext* ctx, std::string_view buffer) noexcept
    : LexerBase(ctx, buffer)
{
}

void Lexer::next(tpl::Token& token)
{
    while (isValid()) {
        if (isSpace(current())) {
            advance();
            continue;
        }

        if (isAlpha(current())) {
            return identifier(token);
        }

        return unexpected(token);
    }

    return make(token, TokenKind::EndOfFile, 0);
}

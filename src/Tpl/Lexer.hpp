//
// Created by Albert on 19/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/LexerBase.hpp"
#include "Token.hpp"

namespace support {
class GlobalContext;
}

namespace tpl {
class Lexer : support::LexerBase<Token> {
public:
    Lexer(support::GlobalContext* ctx, std::string_view buffer) noexcept;
    void next(Token& token);
};

} // namespace tpl
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
    enum class State {
        Text,
        Statement,
        Expression,
    };

    Lexer(support::GlobalContext* ctx, std::string_view buffer) noexcept;
    void next(Token& token);

    [[nodiscard]] auto inline getState() const noexcept { return m_state; }
    [[nodiscard]] auto inline setState(State state) noexcept { m_state = state; }

private:
    void text(Token& token);
    void grammar(Token& token);

    State m_state;
};

} // namespace tpl
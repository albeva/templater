//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/LexerBase.hpp"
#include "Token.hpp"

namespace support {
class Source;
class GlobalContext;
}

namespace table::parser {

class Lexer final : support::LexerBase<Token> {
public:
    explicit Lexer(support::GlobalContext* ctx, support::Source* source) noexcept;
    void next(Token&);
    [[nodiscard]] auto getSource() const noexcept { return m_source; }

private:
    void skipToLineEnd() noexcept;
    void skipToNextLine() noexcept;

    support::Source* m_source;
    bool m_hasStmt = false;
};

} // namespace table::parser

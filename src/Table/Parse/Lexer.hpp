//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater::support {
class Source;
struct SourceLoc;
class Context;
}

namespace templater::table::parser {
struct Token;
enum class TokenKind;

class Lexer final {
public:
    NO_COPY_AND_MOVE(Lexer)
    explicit Lexer(support::Context* ctx, support::Source* source) noexcept;
    ~Lexer() = default;

    void next(Token&);
    [[nodiscard]] auto getSource() const noexcept { return m_source; }

private:
    void skipToLineEnd() noexcept;
    void skipToNextLine() noexcept;

    void make(Token& token, TokenKind kind, std::size_t len = 1) noexcept;
    void unexpected(Token& token, std::string_view message = {}) noexcept;
    void string(Token& token);
    [[nodiscard]] auto escape() noexcept -> std::expected<char, std::string_view>;
    void identifier(Token& token);
    void number(Token& token);

    auto loc(const char* start) noexcept -> support::SourceLoc;

    support::Context* m_ctx;
    support::Source* m_source;
    const char* m_buffer;
    const char* m_input;
    bool m_hasStmt = false;
};

} // namespace templater::table::parser

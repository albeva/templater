//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater {
class Source;
struct SourceLoc;
class Context;
}

namespace templater::table {
class Token;
enum class TokenKind : uint8_t;

class LexerException final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Lexer final {
public:
    NO_COPY_AND_MOVE(Lexer)
    explicit Lexer(Context* ctx, Source* source);
    ~Lexer() = default;

    void next(Token&);
    [[nodiscard]] auto getSource() const { return m_source; }

private:
    void skipToLineEnd();
    void make(Token& token, TokenKind kind, std::size_t len = 1);
    void unexpected(Token& token, std::string_view message = ""sv);

    void string(Token& token);
    [[nodiscard]] auto escape() -> std::expected<char, std::string_view>;
    void identifier(Token& token);
    void number(Token& token);

    Context* m_ctx;
    Source* m_source;
    const char* m_input;
    bool m_hasStmt = false;
    auto loc(const char* start) -> SourceLoc;
    void skipToNextLine();
};

} // namespace templater::table

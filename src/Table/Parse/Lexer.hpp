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

namespace templater::table::parser {
struct Token;
enum class TokenKind : uint8_t;

class Lexer final {
public:
    NO_COPY_AND_MOVE(Lexer)
    explicit Lexer(Context* ctx, Source* source);
    ~Lexer() = default;

    void next(Token&);
    [[nodiscard]] auto getSource() const { return m_source; }

private:
    void skipToLineEnd();
    void skipToNextLine();

    void make(Token& token, TokenKind kind, std::size_t len = 1);
    void unexpected(Token& token, std::string_view message = {});
    void string(Token& token);
    [[nodiscard]] auto escape() -> std::expected<char, std::string_view>;
    void identifier(Token& token);
    void number(Token& token);

    auto loc(const char* start) -> SourceLoc;

    Context* m_ctx;
    Source* m_source;
    const char* m_buffer;
    const char* m_input;
    bool m_hasStmt = false;
};

} // namespace templater::table::parser

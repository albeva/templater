//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater {
class Source;
struct SourceLoc;
}

namespace templater::table {
class Token;
enum class TokenKind;

class LexerException final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Lexer final {
public:
    NO_COPY_AND_MOVE(Lexer)
    explicit Lexer(Source& source);
    ~Lexer() = default;

    void next(Token&);

private:
    void skipToLineEnd();
    void make(Token& token, TokenKind kind, std::size_t len = 1);
    void unexpected(Token& token, std::string_view message = ""sv);

    void string(Token& token);
    void identifier(Token& token);
    void number(Token& token);

    const char* m_input;
    bool m_hasStmt = false;
    SourceLoc loc(const char* start);
    void skipToNextLine();
};

} // namespace templater::table

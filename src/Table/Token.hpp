//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/SourceLoc.hpp"

namespace templater::table {

enum class TokenKind : uint8_t {
    Invalid,
    EndOfFile,
    EndOfLine,
    String,
    Number,
    Identifier,
    ParenOpen,
    ParenClose,
    BraceOpen,
    BraceClose,
    BracketOpen,
    BracketClose,
    Assign,
    Period,
    Plus,
    Equal,
    NotEqual,
    LogicalAnd,
    LogicalOr,
    LogicalNot,
    KwTable,
    KwImport,
    KwAs,
    KwOrder,
    KwBy
};

class Token final {
public:
    void set(TokenKind kind, SourceLoc loc, std::string_view value = ""sv)
    {
        m_kind = kind;
        m_loc = loc;
        m_value = value;
    }

    [[nodiscard]] TokenKind getKind() const { return m_kind; }
    [[nodiscard]] const SourceLoc& getLoc() const { return m_loc; }
    [[nodiscard]] const std::string_view& getValue() const { return m_value; }

    [[nodiscard]] inline bool isValue() const
    {
        return m_kind == TokenKind::Number
            || m_kind == TokenKind::Identifier
            || m_kind == TokenKind::String;
    }

    [[nodiscard]] int getPrecedence() const
    {
        switch (m_kind) {
        case TokenKind::LogicalNot:
            return 4;
        case TokenKind::Equal:
        case TokenKind::NotEqual:
            return 3;
        case TokenKind::LogicalAnd:
            return 2;
        case TokenKind::LogicalOr:
            return 1;
        default:
            return 0;
        }
    }

    [[nodiscard]] std::string_view description() const { return describe(m_kind); }
    [[nodiscard]] static std::string_view describe(TokenKind kind);

    [[nodiscard]] bool is(TokenKind kind) const { return m_kind == kind; }
    template <typename... Ty>
    [[nodiscard]] bool is(TokenKind kind, Ty... ty) const { return is(kind) || is(ty...); }

    [[nodiscard]] bool isNot(TokenKind kind) const { return m_kind != kind; }
    template <typename... Ty>
    [[nodiscard]] bool isNot(TokenKind kind, Ty... ty) const { return isNot(kind) && isNot(ty...); }

private:
    TokenKind m_kind;
    SourceLoc m_loc;
    std::string_view m_value;
};

} // namespace templater::table

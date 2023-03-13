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
    Less,
    LessOrEqual,
    GreaterOrEqual,
    Greater,
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

    [[nodiscard]] auto getKind() const -> TokenKind { return m_kind; }
    [[nodiscard]] auto getLoc() const -> const SourceLoc& { return m_loc; }
    [[nodiscard]] auto getValue() const -> const std::string_view& { return m_value; }

    [[nodiscard]] auto getString() const -> std::string_view;

    [[nodiscard]] inline auto isValue() const -> bool
    {
        return m_kind == TokenKind::Number
            || m_kind == TokenKind::Identifier
            || m_kind == TokenKind::String;
    }

    [[nodiscard]] auto getPrecedence() const -> int
    {
        switch (m_kind) {
        case TokenKind::LogicalNot:
            return 6; // NOLINT
        case TokenKind::Less:
        case TokenKind::LessOrEqual:
        case TokenKind::GreaterOrEqual:
        case TokenKind::Greater:
            return 5; // NOLINT
        case TokenKind::Equal:
            return 4;
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

    [[nodiscard]] auto description() const -> std::string_view { return describe(m_kind); }
    [[nodiscard]] static auto describe(TokenKind kind) -> std::string_view;

    [[nodiscard]] auto is(TokenKind kind) const -> bool { return m_kind == kind; }
    template <typename... Ty>
    [[nodiscard]] auto is(TokenKind kind, Ty... ty) const -> bool { return is(kind) || is(ty...); }

    [[nodiscard]] auto isNot(TokenKind kind) const -> bool { return m_kind != kind; }
    template <typename... Ty>
    [[nodiscard]] auto isNot(TokenKind kind, Ty... ty) const -> bool { return isNot(kind) && isNot(ty...); }

private:
    TokenKind m_kind {};
    SourceLoc m_loc {};
    std::string_view m_value {};
};

} // namespace templater::table

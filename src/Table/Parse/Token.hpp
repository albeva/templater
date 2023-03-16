//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/SourceLoc.hpp"
namespace templater::table::parser {

enum class TokenKind {
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
    Minus,
    Pipe,
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

struct Token final {
    inline void set(TokenKind kind, support::SourceLoc loc, std::string_view value = {}) noexcept
    {
        m_kind = kind;
        m_loc = loc;
        m_value = value;
    }

    [[nodiscard]] inline auto getKind() const noexcept -> const TokenKind& { return m_kind; }
    [[nodiscard]] inline auto getLoc() const noexcept -> const support::SourceLoc& { return m_loc; }
    [[nodiscard]] inline auto getValue() const noexcept -> const std::string_view& { return m_value; }
    [[nodiscard]] inline auto isValue() const noexcept -> bool
    {
        return m_kind == TokenKind::Number
            || m_kind == TokenKind::Identifier
            || m_kind == TokenKind::String;
    }

    [[nodiscard]] inline auto getPrecedence() const noexcept -> int
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

    [[nodiscard]] auto getString() const noexcept -> std::string_view;
    [[nodiscard]] auto description() const noexcept -> std::string_view { return describe(m_kind); }
    [[nodiscard]] static auto describe(TokenKind kind) noexcept -> std::string_view;

    [[nodiscard]] inline auto is(TokenKind kind) const noexcept -> bool { return m_kind == kind; }
    template <typename... Ty>
    [[nodiscard]] inline auto is(TokenKind kind, Ty... ty) const noexcept -> bool { return is(kind) || is(ty...); }

    [[nodiscard]] inline auto isNot(TokenKind kind) const noexcept -> bool { return m_kind != kind; }
    template <typename... Ty>
    [[nodiscard]] inline auto isNot(TokenKind kind, Ty... ty) const noexcept -> bool { return isNot(kind) && isNot(ty...); }

private:
    TokenKind m_kind {};
    support::SourceLoc m_loc {};
    std::string_view m_value {};
};

} // namespace templater::table::parser

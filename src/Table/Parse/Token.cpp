//
// Created by Albert on 05/03/2023.
//
#include "Token.hpp"
using namespace templater::table::parser;

auto Token::describe(TokenKind kind) -> std::string_view
{
    switch (kind) {
    case TokenKind::Invalid:
        return "<Invalid>"sv;
    case TokenKind::EndOfFile:
        return "<End-Of-File>"sv;
    case TokenKind::EndOfLine:
        return "<End-Of-Line>"sv;
    case TokenKind::String:
        return "<String-Literal>"sv;
    case TokenKind::Number:
        return "<Number-Literal>"sv;
    case TokenKind::Identifier:
        return "<Identifier>"sv;
    case TokenKind::ParenOpen:
        return "("sv;
    case TokenKind::ParenClose:
        return ")"sv;
    case TokenKind::BraceOpen:
        return "{"sv;
    case TokenKind::BraceClose:
        return "}"sv;
    case TokenKind::BracketOpen:
        return "["sv;
    case TokenKind::BracketClose:
        return "]"sv;
    case TokenKind::Assign:
        return "="sv;
    case TokenKind::Period:
        return "."sv;
    case TokenKind::Plus:
        return "+"sv;
    case TokenKind::Less:
        return "<"sv;
    case TokenKind::LessOrEqual:
        return "<="sv;
    case TokenKind::GreaterOrEqual:
        return ">="sv;
    case TokenKind::Greater:
        return ">"sv;
    case TokenKind::Equal:
        return "=="sv;
    case TokenKind::NotEqual:
        return "!="sv;
    case TokenKind::LogicalAnd:
        return "&&"sv;
    case TokenKind::LogicalOr:
        return "||"sv;
    case TokenKind::LogicalNot:
        return "!"sv;
    case TokenKind::KwTable:
        return "table"sv;
    case TokenKind::KwImport:
        return "import"sv;
    case TokenKind::KwAs:
        return "as"sv;
    case TokenKind::KwOrder:
        return "order"sv;
    case TokenKind::KwBy:
        return "by"sv;
    default:
        std::unreachable();
    }
}

auto Token::getString() const -> std::string_view
{
    if (isValue() || is(TokenKind::Invalid)) {
        return m_value;
    }
    return description();
}

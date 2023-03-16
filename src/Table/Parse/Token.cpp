//
// Created by Albert on 05/03/2023.
//
#include "Token.hpp"
using templater::table::parser::Token;
using namespace std::literals;

auto Token::describe(TokenKind kind) noexcept -> std::string_view
{
    using enum TokenKind;
    switch (kind) {
    case Invalid:
        return "<Invalid>"sv;
    case EndOfFile:
        return "<End-Of-File>"sv;
    case EndOfLine:
        return "<End-Of-Line>"sv;
    case String:
        return "<String-Literal>"sv;
    case Number:
        return "<Number-Literal>"sv;
    case Identifier:
        return "<Identifier>"sv;
    case ParenOpen:
        return "("sv;
    case ParenClose:
        return ")"sv;
    case BraceOpen:
        return "{"sv;
    case BraceClose:
        return "}"sv;
    case BracketOpen:
        return "["sv;
    case BracketClose:
        return "]"sv;
    case Assign:
        return "="sv;
    case Period:
        return "."sv;
    case Plus:
        return "+"sv;
    case Minus:
        return "-"sv;
    case Pipe:
        return "|"sv;
    case Less:
        return "<"sv;
    case LessOrEqual:
        return "<="sv;
    case GreaterOrEqual:
        return ">="sv;
    case Greater:
        return ">"sv;
    case Equal:
        return "=="sv;
    case NotEqual:
        return "!="sv;
    case LogicalAnd:
        return "&&"sv;
    case LogicalOr:
        return "||"sv;
    case LogicalNot:
        return "!"sv;
    case KwTable:
        return "table"sv;
    case KwImport:
        return "import"sv;
    case KwAs:
        return "as"sv;
    default:
        std::unreachable();
    }
}

auto Token::getString() const noexcept -> std::string_view
{
    if (isValue() || is(TokenKind::Invalid)) {
        return m_value;
    }
    return description();
}

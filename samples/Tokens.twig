{% include "Tokens.tbl" %}
//
// Populate from "Tokens.tbl"
//
#pragma once
#include "pch.hpp"
#include "Support/SourceLoc.hpp"
namespace lbc::parser {

enum class TokenKind {
{% for token in All %}
    {{ token.id }},
{% endfor %}
};

class Token {
    [[nodiscard]] inline auto isOperator() const noexcept {
        return m_token >= TokenKind::{{ Operators.first.id }}
            && m_token <= TokenKind::{{ Operators.last.id }};
    }}

    [[nodiscard]] inline auto getPrecedence() const noexcept {
        switch (m_token) {
        {% for group in Operators.groupBy("precedence"); for token in group %}
        case TokenKind::{{ token.id }}:
            {% if loop.last %}
            return {{ token.precedence }};
            {% endif %}
        {% endfor; endfor %}
        default:
            return 0;
        }
    }

    [[nodiscard]] auto Token::describe(TokenKind kind) noexcept -> std::string_view {
        switch (m_token) {
        {% for token in All %}
        case TokenKind::{{ token.id }}:
            return "{{ token.string }}"sv;
        {% endfor %}
        }
    }

private:
    TokenKind m_token;
};

} // namespace lbc::parser
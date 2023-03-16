//
// Created by Albert on 14/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Table/Parse/Token.hpp"
namespace templater::table {
namespace details {
    struct Tag { };
    struct StringTag { };
    template <typename T, typename X = Tag>
    struct Literal {
        constexpr Literal(support::SourceLoc loc, const T& value)
            : m_value(value)
            , m_loc(loc)
        {
        }

        [[nodiscard]] constexpr auto getLoc() const -> const support::SourceLoc& { return m_loc; }
        [[nodiscard]] constexpr auto getValue() const -> const T& { return m_value; }

    private:
        T m_value;
        support::SourceLoc m_loc;
    };
} // namespace details

using Identifier = details::Literal<std::string_view>;
using NumberLiteral = details::Literal<unsigned>;
using StringLiteral = details::Literal<std::string_view, details::StringTag>;
using Value = std::variant<Identifier, NumberLiteral, StringLiteral>;

[[nodiscard]] constexpr auto toString(const Value& value) -> std::string
{
    return std::visit(
        support::Visitor {
            [](const Identifier& id) {
                return std::string(id.getValue());
            },
            [](const NumberLiteral& num) {
                return std::to_string(num.getValue());
            },
            [](const StringLiteral& str) {
                std::stringstream ss {};
                ss << std::quoted(str.getValue());
                return ss.str();
            },
            [](const PipeLiteral&) {
                return std::string("|");
            } },
        value);
}
} // namespace templater::table

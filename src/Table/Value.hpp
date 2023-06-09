//
// Created by Albert on 14/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/SourceLoc.hpp"
namespace table {
namespace details {
    struct Tag { };
    struct StringTag { };
    template <typename T, typename X = Tag>
    struct Literal {
        Literal(support::SourceLoc loc, const T& value)
            : m_value(value)
            , m_loc(loc)
        {
        }

        [[nodiscard]] inline auto getLoc() const -> const auto& { return m_loc; }
        [[nodiscard]] inline auto getValue() const -> const T& { return m_value; }

    private:
        T m_value;
        support::SourceLoc m_loc;
    };
} // namespace details

using Identifier = details::Literal<std::string_view>;
using NumberLiteral = details::Literal<unsigned>;
using StringLiteral = details::Literal<std::string_view, details::StringTag>;
using Value = std::variant<Identifier, NumberLiteral, StringLiteral>;

[[nodiscard]] inline auto toString(const Value& value) -> std::string
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
            } },
        value);
}
} // namespace table

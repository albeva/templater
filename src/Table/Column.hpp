//
// Created by Albert on 13/03/2023.
//
#pragma once
#include "Support/SourceLoc.hpp"
#include "Table/Parse/Token.hpp"
#include "Value.hpp"
namespace templater::table {
using support::SourceLoc;

class Column final {
public:
    Column(std::string_view name, SourceLoc loc, std::optional<Value> value) noexcept
        : m_name(name)
        , m_loc(loc)
        , m_value(value)
    {
    }

    [[nodiscard]] inline auto getName() const noexcept -> const std::string_view& { return m_name; }
    [[nodiscard]] inline auto getLoc() const noexcept -> const SourceLoc& { return m_loc; }
    [[nodiscard]] inline auto getValue() const noexcept -> const std::optional<Value>& { return m_value; }

private:
    std::string_view m_name;
    SourceLoc m_loc;
    std::optional<Value> m_value;
};

} // namespace templater::table

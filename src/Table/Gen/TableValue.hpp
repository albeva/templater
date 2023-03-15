//
// Created by Albert on 14/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Table/Value.hpp"

namespace templater::table {
struct Token;
namespace ast {
    struct Literal;
    struct StructBody;
}
}

namespace templater::table::gen {
struct TableValue final {
    [[nodiscard]] auto operator()(const Identifier& node) const -> Value;
    [[nodiscard]] auto operator()(const StringLiteral& node) const -> Value;
    [[nodiscard]] auto operator()(const NumberLiteral& node) const -> Value;
    [[nodiscard]] auto operator()(const ast::StructBody* node) const -> Value;
};
}

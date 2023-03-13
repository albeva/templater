//
// Created by Albert on 13/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Symbol.hpp"

namespace templater {
class Context;
}

namespace templater::table {
class Symbol;

class SymbolTable final {
public:
    NO_COPY_AND_MOVE(SymbolTable)
    explicit SymbolTable(Context* ctx);
    ~SymbolTable() = default;

    [[nodiscard]] auto find(std::string_view name) const -> Symbol*;
    [[nodiscard]] auto contains(std::string_view name) const -> bool;
    void insert(Symbol* symbol);

private:
    struct Comparison {
        using is_transparent = std::true_type;

        auto operator()(const Symbol* lhs, const Symbol* rhs) const -> bool
        {
            return lhs->getName() == rhs->getName();
        }

        auto operator()(const Symbol* lhs, std::string_view rhs) const -> bool
        {
            return lhs->getName() == rhs;
        }

        auto operator()(std::string_view lhs, const Symbol* rhs) const -> bool
        {
            return lhs == rhs->getName();
        }
    };

    struct Hash {
        using is_transparent = std::true_type;
        using hash = std::hash<std::string_view>;

        [[nodiscard]] auto operator()(const Symbol* sym) const -> size_t
        {
            return hash {}(sym->getName());
        }

        [[nodiscard]] auto operator()(std::string_view name) const -> size_t
        {
            return hash {}(name);
        }
    };

    std::pmr::unordered_set<Symbol*, Hash, Comparison> m_symbols;
};

} // namespace templater::table

//
// Created by Albert on 13/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Symbol.hpp"

namespace support {
class Source;
}

namespace table {
class Symbol;

class SymbolTable final {
public:
    NO_COPY_AND_MOVE(SymbolTable)
    explicit SymbolTable(support::Source* source);
    ~SymbolTable();

    /**
     * Allocate and create ast node T. Destructor is never called symbol table objects
     * instead whole memory pool is freed at once when SymbolTable deallocates.
     */
    template <class T, class... Args>
    [[nodiscard]] inline auto create(Args&&... args) -> T*
    {
        auto obj = m_pa.allocate_object<T>();
        return std::construct_at(obj, std::forward<Args>(args)...);
    }

    [[nodiscard]] inline auto getAllocator() const noexcept -> auto& { return m_pa; }

    [[nodiscard]] auto getSymbols() const noexcept -> const auto& { return m_symbols; }
    [[nodiscard]] auto find(std::string_view name) const noexcept -> Symbol*;
    [[nodiscard]] auto contains(std::string_view name) const noexcept -> bool;
    void insert(Symbol* symbol);

    [[nodiscard]] auto getSource() const noexcept { return m_source; }

private:
    support::Source* m_source;
    std::pmr::monotonic_buffer_resource m_mbr;
    std::pmr::polymorphic_allocator<std::byte> m_pa;

    struct Comparison {
        using is_transparent = std::true_type;

        auto operator()(const Symbol* lhs, const Symbol* rhs) const noexcept -> bool
        {
            return lhs->getName() == rhs->getName();
        }

        auto operator()(const Symbol* lhs, std::string_view rhs) const noexcept -> bool
        {
            return lhs->getName() == rhs;
        }

        auto operator()(std::string_view lhs, const Symbol* rhs) const noexcept -> bool
        {
            return lhs == rhs->getName();
        }
    };

    struct Hash {
        using is_transparent = std::true_type;
        using hash = std::hash<std::string_view>;

        [[nodiscard]] auto operator()(const Symbol* sym) const noexcept -> size_t
        {
            return hash {}(sym->getName());
        }

        [[nodiscard]] auto operator()(std::string_view name) const noexcept -> size_t
        {
            return hash {}(name);
        }
    };

    std::pmr::unordered_set<Symbol*, Hash, Comparison> m_symbols;
};

} // namespace table

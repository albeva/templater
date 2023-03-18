//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
#include "Support/GlobalContext.hpp"
namespace table::ast {

/**
 * Allocator for Ast nodes.
 * No destructors are called for Ast nodes,
 * instead memory is simply released when owning Context
 * is deallocated.
 */
class Allocator final {
public:
    explicit Allocator(support::GlobalContext* context) noexcept
        : m_context(context)
    {
    }

    template <class T, class... Args>
    [[nodiscard]] inline auto node(Args&&... args) -> T*
    {
        return m_context->create<T>(std::forward<Args>(args)...);
    }

    template <class T>
    [[nodiscard]] inline auto list() -> List<T>
    {
        return List<T>(m_context->getAllocator());
    }

private:
    support::GlobalContext* m_context;
};
} // namespace table::ast

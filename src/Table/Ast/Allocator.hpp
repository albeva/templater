//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
#include "Support/Context.hpp"
namespace templater::table::ast {

class Allocator final {
public:
    explicit Allocator(support::Context* context)
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
    support::Context* m_context;
};
} // namespace templater::table::ast

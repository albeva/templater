//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
#include "Support/Context.hpp"

namespace templater::table::ast {
class AstAllocator final {
public:
    explicit AstAllocator(Context& context)
        : m_context(context)
    {
    }

    template <class T, class... Args>
    [[nodiscard]] inline T* node(Args&&... args)
    {
        return m_context.create<T>(std::forward<Args>(args)...);
    }

    template <class T>
    [[nodiscard]] inline List<T> list()
    {
        return m_context.vector<T*>();
    }

private:
    Context& m_context;
};
}
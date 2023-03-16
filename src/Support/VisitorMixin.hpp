//
// Created by Albert on 15/03/2023.
//
#pragma once

/// One day this can be replaced with `this` deduce feature in C++23
#define VISITOR_MIXIN                                             \
    /* For visiting with std::visit, map operator() to visit() */ \
    inline auto operator()(const auto& node)                      \
    {                                                             \
        return visit(node);                                       \
    }                                                             \
    /* Visit variant node with std::visit */                      \
    template <typename... Ts>                                     \
    inline auto visit(const std::variant<Ts...>& node)            \
    {                                                             \
        return std::visit(*this, node);                           \
    }                                                             \
    /* visit a range (e.g. vector<Node>) of nodes */              \
    void inline visitEach(const std::ranges::range auto& list)    \
    {                                                             \
        for (const auto& node : list) {                           \
            visit(node);                                          \
        }                                                         \
    }

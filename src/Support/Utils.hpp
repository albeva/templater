//
// Created by Albert on 05/03/2023.
//
#pragma once

// NOLINTBEGIN bugprone-macro-parentheses
#define NO_COPY_AND_MOVE(Class)               \
    Class(Class&&) = delete;                  \
    Class(const Class&) = delete;             \
    auto operator=(Class&&)->Class& = delete; \
    auto operator=(const Class&)->Class& = delete;

#define NO_NEW_AND_DELETE                             \
    auto operator new(size_t)->void* = delete;        \
    auto operator new[](std::size_t)->void* = delete; \
    void operator delete(void*) = delete;

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __COUNTER__)

namespace support {
/**
 * Helper for creating visitors to be used with std::visit
 */
template <typename... Base>
struct Visitor : Base... {
    using Base::operator()...;
};
template <typename... Base>
Visitor(Base...) -> Visitor<Base...>;

template <std::invocable T>
struct ScopeGuard final {
    NO_COPY_AND_MOVE(ScopeGuard)
    constexpr explicit ScopeGuard(T&& handler)
        : handler { std::forward<T>(handler) }
    {
    }

    ~ScopeGuard()
    {
        handler();
    }

private:
    const T handler;
};
template <typename F>
ScopeGuard(F&&) -> ScopeGuard<F>;

/**
 * Execute given statement when existing the scope
 *
 * SCOPE_GAURD(closeFile(file))
 */
#define SCOPE_GAURD(BODY)                             \
    support::ScopeGuard MAKE_UNIQUE(tmp_scope_giard_) \
    {                                                 \
        [&]() BODY                                    \
    } /* NOLINT */

// NOLINTEND
} // namespace support
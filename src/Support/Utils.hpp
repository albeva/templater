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

// NOLINTEND

namespace support {

template <typename... Base>
struct Visitor : Base... {
    using Base::operator()...;
};
template <typename... Base>
Visitor(Base...) -> Visitor<Base...>;

} // namespace support
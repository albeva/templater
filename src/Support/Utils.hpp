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
// NOLINTEND

namespace templater::support {

template <typename... Base>
struct Visitor : Base... {
    using Base::operator()...;
};
template <typename... Base>
Visitor(Base...) -> Visitor<Base...>;

} // namespace templater::support
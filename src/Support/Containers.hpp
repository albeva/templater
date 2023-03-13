//
// Created by Albert on 13/03/2023.
//
#pragma once
#include "pch.hpp"

namespace templater {
namespace containers {
    struct HeterogeneousHash {
        using is_transparent = void;
        [[nodiscard]] auto operator()(const char* txt) const -> size_t
        {
            return std::hash<std::string_view> {}(txt);
        }
        [[nodiscard]] auto operator()(std::string_view txt) const -> size_t
        {
            return std::hash<std::string_view> {}(txt);
        }
        [[nodiscard]] auto operator()(const std::string& txt) const -> size_t
        {
            return std::hash<std::string> {}(txt);
        }
    };
} // namespace containers

using StringSet = std::unordered_set<std::string, containers::HeterogeneousHash, std::equal_to<>>;

template <typename T>
using StringMap = std::unordered_map<std::string, T, StringSet, std::equal_to<>>;

namespace pmr {
    using StringSet = std::pmr::unordered_set<std::pmr::string, containers::HeterogeneousHash, std::equal_to<>>;

    template <typename T>
    using StringMap = std::pmr::unordered_map<std::pmr::string, T, StringSet, std::equal_to<>>;
}

} // namespace templater

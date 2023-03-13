//
// Created by Albert on 04/03/2023.
//
#pragma once
#include <array>
#include <cassert>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#if defined(__GNUG__) && __has_include(<experimental/algorithm>)
#include <experimental/algorithm>
#endif

// 3rd party libs
#include <fmt/core.h>

namespace templater {
using namespace std::literals;
} // namespace templater

// templater headers
#include "Support/Utils.hpp"

//
// Created by Albert on 04/03/2023.
//
#pragma once
#include <array>
#include <cassert>
#include <charconv>
#include <exception>
#include <expected>
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
#include <version>

#if defined(__GNUG__) && __has_include(<experimental/algorithm>)
#include <experimental/algorithm>
#endif

// 3rd party libs
// replace with std <format> and <print> when C++23 matures
#include <fmt/core.h>
#include <fmt/ostream.h>

// templater headers
#include "Support/Containers.hpp"
#include "Support/Utils.hpp"

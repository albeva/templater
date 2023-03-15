//
// Created by Albert on 14/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Table/Parse/Token.hpp"

namespace templater::table {
using Value = std::variant<parser::Token>;
}

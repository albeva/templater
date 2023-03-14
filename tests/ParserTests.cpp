//
// Created by Albert on 14/03/2023.
//
#include "CompilerBase.hpp"
#include "gtest/gtest.h"

// NOLINTBEGIN (cppcoreguidelines-avoid-non-const-global-variables,
//              modernize-use-trailing-return-type,
//              cppcoreguidelines-avoid-magic-numbers,
//              cppcoreguidelines-owning-memory)

namespace {
using templater::table::ParserException;
using templater::tests::CompilerBase;
using templater::tests::enumerate;

TEST_P(CompilerBase, ParserFails)
{
    EXPECT_THROW(parse(), ParserException);
    EXPECT_EQ(expected(), reality());
}

INSTANTIATE_TEST_SUITE_P(
    TestFails, CompilerBase,
    testing::ValuesIn(enumerate("fails/parser/")));
}
// NOLINTEND

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
using table::parser::ParserException;
using tests::CompilerBase;

struct ParserTests : CompilerBase { };

TEST_P(ParserTests, ParserFails)
{
    EXPECT_THROW(parse(), ParserException);
    EXPECT_EQ(expected(), reality());
}

INSTANTIATE_TEST_SUITE_P(
    TestParserFails, ParserTests,
    testing::ValuesIn(CompilerBase::enumerate("fails/parser/")));
}
// NOLINTEND

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
using templater::table::gen::GeneratorException;
using templater::tests::CompilerBase;

struct GeneratorTests: CompilerBase{};

TEST_P(GeneratorTests, GeneratorFails)
{
    EXPECT_THROW(gen(), GeneratorException);
    EXPECT_EQ(expected(), reality());
}

INSTANTIATE_TEST_SUITE_P(
    TestGeneratorFails, GeneratorTests,
    testing::ValuesIn(CompilerBase::enumerate("fails/gen/")));
}
// NOLINTEND

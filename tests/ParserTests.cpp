//
// Created by Albert on 14/03/2023.
//
#include "Support/Context.hpp"
#include "Support/Diagnostics.hpp"
#include "Support/Source.hpp"
#include "Table/Lexer.hpp"
#include "Table/Parser.hpp"
#include "gtest/gtest.h"

// NOLINTBEGIN (cppcoreguidelines-avoid-non-const-global-variables,
//              modernize-use-trailing-return-type,
//              cppcoreguidelines-avoid-magic-numbers,
//              cppcoreguidelines-owning-memory)

namespace {
using namespace templater;
using namespace table;

struct ParserTests : public testing::TestWithParam<std::filesystem::path> {
    void SetUp() override
    {
        m_source = std::make_unique<Source>(GetParam());
        m_lexer = std::make_unique<Lexer>(&m_ctx, m_source.get());
        m_parser = std::make_unique<Parser>(&m_ctx, &m_diag, m_lexer.get());
    }

    Context m_ctx;
    Diagnostics m_diag;
    std::unique_ptr<Source> m_source;
    std::unique_ptr<Lexer> m_lexer;
    std::unique_ptr<Parser> m_parser;
};

TEST_P(ParserTests, TestFail)
{
    EXPECT_EQ(true, true);
}

INSTANTIATE_TEST_SUITE_P(TestFails, ParserTests,
    testing::Values(
        "fails/parser/unexpected_tokens.tbl",
        "fails/parser/unexpected_tokens2.tbl"));
}

// NOLINTEND

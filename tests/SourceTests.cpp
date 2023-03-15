//
// Created by Albert on 11/03/2023.
//
#include "Support/Source.hpp"
#include "Support/SourceLoc.hpp"
#include "gtest/gtest.h"

// NOLINTBEGIN (cppcoreguidelines-avoid-non-const-global-variables,
//              modernize-use-trailing-return-type,
//              cppcoreguidelines-avoid-magic-numbers,
//              cppcoreguidelines-owning-memory)

namespace {
using templater::Source;
using templater::SourceLoc;

class SourceTests : public testing::Test {
protected:
    void load(std::string_view src)
    {
        source = std::make_unique<Source>("unnamed", std::string(src));
    }
    std::unique_ptr<Source> source;
};

TEST_F(SourceTests, Empty)
{
    load("");
    EXPECT_EQ(source->length(), 0);
    EXPECT_NE(source->data(), nullptr);
    EXPECT_EQ(std::string(source->data()), "");
}

TEST_F(SourceTests, GetLine)
{
    static constexpr auto src = "hello\n"
                                "world\n"
                                "!";
    load(src);

    EXPECT_EQ(source->length(), 13);
    EXPECT_EQ(source->getLine(1), "hello");
    EXPECT_EQ(source->getLine(2), "world");
    EXPECT_EQ(source->getLine(3), "!");
}

TEST_F(SourceTests, GetLoc)
{
    static constexpr auto src = "hello\n"
                                " world \n"
                                "!";
    load(src);

    {
        auto loc = SourceLoc(0, 5);
        auto pos = source->getPosition(loc);
        EXPECT_EQ(pos.getLine(), 1);
        EXPECT_EQ(pos.getCol(), 1);
        EXPECT_EQ(pos.getLength(), 5);
        EXPECT_EQ(source->getString(loc), "hello");
    }

    {
        auto loc = SourceLoc(7, 12);
        auto pos = source->getPosition(loc);
        EXPECT_EQ(pos.getLine(), 2);
        EXPECT_EQ(pos.getCol(), 2);
        EXPECT_EQ(pos.getLength(), 5);
        EXPECT_EQ(source->getString(loc), "world");
    }
}
}

// NOLINTEND

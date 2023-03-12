//
// Created by Albert on 11/03/2023.
//
#include "Support/Source.hpp"
#include "Support/SourceLoc.hpp"
#include "gtest/gtest.h"
namespace {
using namespace templater;

class SourceTests : public testing::Test {
protected:
    void load(std::string_view src) {
        source = std::make_unique<Source>("unnamed", std::string(src));
    }
    std::unique_ptr<Source> source;
};

TEST_F(SourceTests, Empty) { // NOLINT
    load("");
    EXPECT_EQ(source->length(), 0);
    EXPECT_NE(source->data(), nullptr);
    EXPECT_EQ(std::string(source->data()), ""s);
}

TEST_F(SourceTests, GetLine) { // NOLINT
    static constexpr auto src =
        "hello\n"
        "world\n"
        "!";
    load(src);

    EXPECT_EQ(source->length(), 13);
    EXPECT_EQ(source->getString(1), "hello"s);
    EXPECT_EQ(source->getString(2), "world"s);
    EXPECT_EQ(source->getString(3), "!"s);
}

TEST_F(SourceTests, GetLoc) { // NOLINT
    static constexpr auto src =
        "hello\n"
        " world \n"
        "!";
    load(src);

    {
        const auto* to = source->data();
        std::advance(to, 5); // NOLINT

        auto loc = SourceLoc(source->data(), to);
        auto [line, col] = source->getLineAndCol(loc);
        EXPECT_EQ(line, 1);
        EXPECT_EQ(col, 1);
        EXPECT_EQ(loc.length(), 5);
        EXPECT_EQ(source->getString(loc), "hello"s);
    }

    {
        const auto *from = source->data();
        std::advance(from, 7); // NOLINT
        const auto* to = from;
        std::advance(to, 5);   // NOLINT

        auto loc = SourceLoc(from, to);
        auto [line, col] = source->getLineAndCol(loc);
        EXPECT_EQ(line, 2);
        EXPECT_EQ(col, 2);
        EXPECT_EQ(loc.length(), 5);
        EXPECT_EQ(source->getString(loc), "world"s);
    }
}
}
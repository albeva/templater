//
// Created by Albert on 11/03/2023.
//
#include "Support/Source.hpp"
#include "Support/SourceLoc.hpp"
#include "Support/Context.hpp"
#include "Table/Lexer.hpp"
#include "Table/Token.hpp"
#include "gtest/gtest.h"

// NOLINTBEGIN (cppcoreguidelines-avoid-non-const-global-variables,
//              modernize-use-trailing-return-type,
//              cppcoreguidelines-avoid-magic-numbers,
//              cppcoreguidelines-owning-memory)

namespace {
using namespace templater;
using namespace templater::table;

class LexerTests : public testing::Test {
protected:
    void load(std::string_view source)
    {
        m_ctx = std::make_unique<Context>();
        m_source = std::make_unique<Source>("unnamed", std::string(source));
        m_lexer = std::make_unique<Lexer>(m_ctx.get(), m_source.get());
    }

    void expect(TokenKind kind, std::string_view lexeme = "", unsigned line = 0, unsigned col = 0, unsigned len = 0)
    {
        table::Token token;
        m_lexer->next(token);
        EXPECT_EQ(token.getKind(), kind);

        if (!lexeme.empty()) {
            if (token.isValue() || token.is(TokenKind::Invalid)) {
                EXPECT_EQ(token.getValue(), lexeme);
            } else {
                EXPECT_EQ(token.description(), lexeme);
            }
        }

        auto pos = m_source->getPosition(token.getLoc());
        if (line > 0) {
            EXPECT_EQ(pos.getLine(), line);
        }
        if (col > 0) {
            EXPECT_EQ(pos.getCol(), col);
        }
        if (len > 0) {
            EXPECT_EQ(len, pos.getLength());
        }
    }

private:
    std::unique_ptr<Context> m_ctx;
    std::unique_ptr<table::Lexer> m_lexer;
    std::unique_ptr<Source> m_source;
};

#define EXPECT_TOKEN(KIND, ...)      \
    {                                \
        SCOPED_TRACE(#KIND);         \
        expect(KIND, ##__VA_ARGS__); \
    }

TEST_F(LexerTests, Empty)
{
    static constexpr std::array inputs {
        "",
        "   ",
        "\t\t",
        "\n   \n   ",
        "\r\n",
        "   \r   \n  \t  ",
        "#comment string",
        " \t _ this should be ignored \n_ ignored again"
    };
    for (const auto* input : inputs) {
        load(input);
        EXPECT_TOKEN(TokenKind::EndOfFile)
    }
}

TEST_F(LexerTests, Invalid)
{
    static constexpr auto source = "&|@\n"
                                   "\"open string\n"
                                   "\"open string";
    load(source);

    EXPECT_TOKEN(TokenKind::Invalid, "'&'. Did you mean '&&'?", 1, 1, 1)
    EXPECT_TOKEN(TokenKind::Invalid, "'|'. Did you mean '||'?", 1, 2, 1)
    EXPECT_TOKEN(TokenKind::Invalid, "@", 1, 3, 1)
    EXPECT_TOKEN(TokenKind::Invalid, "open string", 2, 13, 1)
    EXPECT_TOKEN(TokenKind::Invalid, "open string", 3, 13, 0)

    EXPECT_TOKEN(TokenKind::EndOfFile, "", 3, 13, 0)
}

TEST_F(LexerTests, StringEscape)
{
    static constexpr auto source = R"TPL(
"foo \"bar\""
"world \\"
)TPL";
    load(source);

    EXPECT_TOKEN(TokenKind::String, "foo \"bar\"",   2, 1, 13)
    EXPECT_TOKEN(TokenKind::EndOfLine)
    EXPECT_TOKEN(TokenKind::String, "world \\",      3, 1, 10)
}

TEST_F(LexerTests, Stream)
{
    static constexpr auto source = "one \"two\" three 42 = ( ) []\n"
                                   "# comment should be ignored\n"
                                   ".+== != &&|| !\n"
                                   "table import as order by\n"
                                   "Table iMport AS orderby\n"
                                   "foo _ bar \n"
                                   "__foo _bar";
    load(source);

    // clang-format off

    // line 1
    EXPECT_TOKEN(TokenKind::Identifier,   "one",   1, 1,  3)
    EXPECT_TOKEN(TokenKind::String,       "two",   1, 5,  5)
    EXPECT_TOKEN(TokenKind::Identifier,   "three", 1, 11, 5)
    EXPECT_TOKEN(TokenKind::Number,       "42",    1, 17, 2)
    EXPECT_TOKEN(TokenKind::Assign,       "=",     1, 20, 1)
    EXPECT_TOKEN(TokenKind::ParenOpen,    "(",     1, 22, 1)
    EXPECT_TOKEN(TokenKind::ParenClose,   ")",     1, 24, 1)
    EXPECT_TOKEN(TokenKind::BracketOpen,  "[",     1, 26, 1)
    EXPECT_TOKEN(TokenKind::BracketClose, "]",     1, 27, 1)
    EXPECT_TOKEN(TokenKind::EndOfLine,    "",      1, 28, 0)

    // line 3
    EXPECT_TOKEN(TokenKind::Period,       ".",     3, 1,  1)
    EXPECT_TOKEN(TokenKind::Plus,         "+",     3, 2,  1)
    EXPECT_TOKEN(TokenKind::Equal,        "==",    3, 3,  2)
    EXPECT_TOKEN(TokenKind::NotEqual,     "!=",    3, 6,  2)
    EXPECT_TOKEN(TokenKind::LogicalAnd,   "&&",    3, 9,  2)
    EXPECT_TOKEN(TokenKind::LogicalOr,    "||",    3, 11, 2)
    EXPECT_TOKEN(TokenKind::LogicalNot,   "!",     3, 14, 1)
    EXPECT_TOKEN(TokenKind::EndOfLine,    "",      3, 15, 0)

    // line 4
    EXPECT_TOKEN(TokenKind::KwTable,      "table",  4, 1,  5)
    EXPECT_TOKEN(TokenKind::KwImport,     "import", 4, 7,  6)
    EXPECT_TOKEN(TokenKind::KwAs,         "as",     4, 14, 2)
    EXPECT_TOKEN(TokenKind::KwOrder,      "order",  4, 17, 5)
    EXPECT_TOKEN(TokenKind::KwBy,         "by",     4, 23, 2)
    EXPECT_TOKEN(TokenKind::EndOfLine,    "",       4, 25, 0)

    // line 5
    EXPECT_TOKEN(TokenKind::Identifier,   "Table",   5, 1,  5)
    EXPECT_TOKEN(TokenKind::Identifier,   "iMport",  5, 7,  6)
    EXPECT_TOKEN(TokenKind::Identifier,   "AS",      5, 14, 2)
    EXPECT_TOKEN(TokenKind::Identifier,   "orderby", 5, 17, 7)
    EXPECT_TOKEN(TokenKind::EndOfLine,    "",        5, 24, 0)

    // line 6 & 7
    EXPECT_TOKEN(TokenKind::Identifier,   "foo",     6, 1,  3)
    EXPECT_TOKEN(TokenKind::Identifier,   "__foo",   7, 1,  5)
    EXPECT_TOKEN(TokenKind::Identifier,   "_bar",    7, 7,  4)
    EXPECT_TOKEN(TokenKind::EndOfLine,    "",        7, 11, 0)

    // end
    EXPECT_TOKEN(TokenKind::EndOfFile,    "",        7, 11, 0)

    // clang-format on
}
}

// NOLINTEND

//
// Created by Albert on 14/03/2023.
//
#pragma once
#include "Support/Context.hpp"
#include "Support/Diagnostics.hpp"
#include "Support/Source.hpp"
#include "Table/Lexer.hpp"
#include "Table/Parser.hpp"
#include "gtest/gtest.h"

namespace templater::tests {
using templater::table::Lexer;
using templater::table::Parser;

struct CompilerBase : testing::TestWithParam<std::filesystem::path> {
    void SetUp() override
    {
        m_source = std::make_unique<Source>(GetParam());
    }

    auto parse() -> const auto*
    {
        Lexer lexer { &m_ctx, m_source.get() };
        Parser parser { &m_ctx, &m_diag, &lexer };
        return parser.parse();
    }

    [[nodiscard]] auto expected() const -> std::string
    {
        static constexpr auto prefix = "# CHECK: "sv;
        std::stringstream checks {};
        try {
            for (unsigned line = 1;; line++) {
                auto str = m_source->getLine(line);
                if (str.starts_with(prefix)) {
                    checks << str.substr(prefix.length()) << '\n';
                }
            }
        } catch (SourceException&) {
        }
        return checks.str();
    }

    [[nodiscard]] auto reality() const -> auto { return m_output.str(); }

private:
    std::stringstream m_output {};
    Diagnostics m_diag { m_output };
    Context m_ctx;
    std::unique_ptr<Source> m_source;
};

auto enumerate(const std::filesystem::path& base) -> std::vector<std::filesystem::path>
{
    return std::views::all(std::filesystem::directory_iterator { base })
        | std::ranges::to<std::vector<std::filesystem::path>>();
}

} // namespace templater::tests

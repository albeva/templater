//
// Created by Albert on 14/03/2023.
//
#pragma once
#include "Support/Diagnostics.hpp"
#include "Support/GlobalContext.hpp"
#include "Support/Source.hpp"
#include "Table/Ast/Ast.hpp"
#include "Table/Ast/Context.hpp"
#include "Table/Gen/Generator.hpp"
#include "Table/Parse/Lexer.hpp"
#include "Table/Parse/Parser.hpp"
#include "Table/SymbolTable.hpp"
#include "gtest/gtest.h"

namespace tests {
using namespace std::literals;
using support::Diagnostics;
using support::Source;
using support::SourceException;
using support::GlobalContext;
using table::gen::Generator;
using table::parser::Lexer;
using table::parser::Parser;
using table::parser::TokenKind;

struct CompilerBase : testing::TestWithParam<std::filesystem::path> {
    void SetUp() override
    {
        m_source = m_ctx.load(GetParam());
    }

    auto parse()
    {
        Lexer lexer { &m_ctx, m_source };
        Parser parser { &m_ctx, &m_diag, &lexer };
        return parser.parse();
    }

    auto gen()
    {
        auto ast = parse();
        Generator gen { &m_ctx, &m_diag };
        return gen.visit(ast.get());
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

    static auto enumerate(const std::filesystem::path& base) -> std::vector<std::filesystem::path>
    {
        std::vector<std::filesystem::path> paths;
        std::ranges::for_each(std::filesystem::directory_iterator { base },
            [&](const auto& dir_entry) {
                paths.push_back(dir_entry);
            });
        return paths;
    }

private:
    std::stringstream m_output {};
    Diagnostics m_diag { m_output };
    GlobalContext m_ctx;
    Source* m_source = nullptr;
};

} // namespace tests

//
// Created by Albert on 17/03/2023.
//
#include "Driver.hpp"
#include "Ast/Context.hpp"
#include "Ast/Printer.hpp"
#include "Gen/Generator.hpp"
#include "Parse/Lexer.hpp"
#include "Parse/Parser.hpp"
#include "Printer.hpp"
#include "Support/Diagnostics.hpp"
#include "Support/GlobalContext.hpp"
#include "Support/Source.hpp"
#include "SymbolTable.hpp"

using table::Driver;
using table::gen::Generator;
using table::parser::Lexer;
using table::parser::Parser;

Driver::Driver()
    : m_context(std::make_unique<support::GlobalContext>())
    , m_diag(std::make_unique<support::Diagnostics>(std::cerr))
{
}

Driver::~Driver() = default;

auto Driver::parse(const std::filesystem::path& path) -> std::unique_ptr<ast::Context>
{
    auto* src = m_context->load(path);
    Lexer lexer { m_context.get(), src };
    return Parser { m_context.get(), m_diag.get(), &lexer }.parse();
}

auto Driver::compile(const std::filesystem::path& path) -> support::GlobalContext::UniquePtr<SymbolTable>
{
    auto ast = parse(path);
    Generator gen { m_context.get(), m_diag.get() };
    return gen.visit(ast.get());
}

void Driver::printAst(const std::filesystem::path& path)
{
    auto ast = parse(path);
    std::cout << ast::Printer { ast.get() };
}

void Driver::printTable(const std::filesystem::path& path)
{
    auto table = compile(path);
    std::cout << Printer { table.get() };
}

auto table::Driver::exitCode() const noexcept -> int
{
    return m_diag->hasErrors() ? EXIT_FAILURE : EXIT_SUCCESS;
}

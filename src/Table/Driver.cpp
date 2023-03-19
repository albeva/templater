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
#include "Support/GlobalContext.hpp"
#include "Support/Source.hpp"
#include "SymbolTable.hpp"

using table::Driver;
using table::gen::Generator;
using table::parser::Lexer;
using table::parser::Parser;

Driver::Driver(support::GlobalContext* ctx)
    : m_context(ctx)
{
}

Driver::~Driver() = default;

auto Driver::parse(const std::filesystem::path& path) -> std::unique_ptr<ast::Context>
{
    auto* src = m_context->load(path);
    Lexer lexer { m_context, src };
    return Parser { m_context->getDiagnostics(), &lexer }.parse();
}

auto Driver::compile(const std::filesystem::path& path) -> std::unique_ptr<SymbolTable>
{
    auto ast = parse(path);
    return Generator(m_context->getDiagnostics()).visit(ast.get());
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

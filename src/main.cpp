//
// Created by Albert on 04/03/2023.
//
#include "Support/Diagnostics.hpp"
#include "Support/Source.hpp"
#include "Table/Ast/Ast.hpp"
#include "Table/Ast/Printer.hpp"
#include "Table/Gen/Generator.hpp"
#include "Table/Parse/Lexer.hpp"
#include "Table/Parse/Parser.hpp"
#include "Table/Printer.hpp"
using templater::support::Context;
using templater::support::Diagnostics;
using templater::support::Source;
using templater::table::Printer;
using templater::table::gen::Generator;
using templater::table::parser::Lexer;
using templater::table::parser::Parser;

auto main() -> int
{
    try {
        Diagnostics diag { std::cerr };
        Context ctx {};
        Source src { "../samples/Simple.tbl" };
        Lexer lexer { &ctx, &src };
        Parser parser { &ctx, &diag, &lexer };
        auto* ast = parser.parse();
        Generator const gen { &ctx, &diag, &src, ast };
        std::cout << Printer(gen.getSymbolTable());
        // std::cout << templater::table::ast::Printer(ast) << '\n';
        return diag.hasErrors() ? EXIT_FAILURE : EXIT_SUCCESS;
    } catch (std::exception& exc) {
        std::cerr << exc.what();
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_FAILURE;
    }
}
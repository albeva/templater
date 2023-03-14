//
// Created by Albert on 04/03/2023.
//
#include "Support/Diagnostics.hpp"
#include "Support/Source.hpp"
#include "Table/Ast.hpp"
#include "Table/Generator.hpp"
#include "Table/Lexer.hpp"
#include "Table/Parser.hpp"
#include "Table/Printer.hpp"
using namespace templater;
using namespace table;
using namespace ast;

auto main() -> int
{
    try {
        Diagnostics diag {};
        Context ctx {};
        Source src { "../tests/tables/fails/parser/unexpected_tokens2.tbl" };
        Lexer lexer { &ctx, &src };
        Parser parser { &ctx, &diag, &lexer };
        auto* ast = parser.parse();
        Generator { &ctx, ast };
        return diag.hasErrors() ? EXIT_FAILURE : EXIT_SUCCESS;
    } catch (std::exception& exc) {
        std::cerr << exc.what();
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_FAILURE;
    }
}
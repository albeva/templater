//
// Created by Albert on 04/03/2023.
//
#include "Support/Source.hpp"
#include "Table/Ast.hpp"
#include "Table/Lexer.hpp"
#include "Table/Parser.hpp"
#include "Table/Printer.hpp"
using namespace templater;
using namespace table;
using namespace ast;

auto main() -> int
{
    try {
        Source src { "../samples/tokens.tbl" };
        Lexer lexer { src };
        Context ctx {};
        Parser parser { ctx, lexer };
        auto* ast = parser.parse();
        std::cout << Printer(*ast) << '\n';
        return EXIT_SUCCESS;
    } catch (std::exception& exc) {
        std::cerr << "templater error: " << exc.what() << '\n';
        return EXIT_FAILURE;
    }
}
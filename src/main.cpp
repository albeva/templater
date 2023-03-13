//
// Created by Albert on 04/03/2023.
//
#include "Support/Source.hpp"
#include "Table/Ast.hpp"
#include "Table/Generator.hpp"
#include "Table/Lexer.hpp"
#include "Table/Parser.hpp"
using namespace templater;
using namespace table;
using namespace ast;

auto main() -> int
{
    try {
        Context ctx {};
        Source src { "../samples/Tokens.tbl" };
        Lexer lexer { &ctx, &src };
        Parser parser { &ctx, &lexer };
        auto* ast = parser.parse();
        Generator { &ctx, *ast };
        return EXIT_SUCCESS;
    } catch (std::exception& exc) {
        std::cerr << "error: " << exc.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_FAILURE;
    }
}
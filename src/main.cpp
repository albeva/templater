//
// Created by Albert on 04/03/2023.
//
#include "Support/Diagnostics.hpp"
#include "Support/GlobalContext.hpp"
#include "Support/Source.hpp"
#include "Table/Driver.hpp"
#include "Table/SymbolTable.hpp"
#include "Tpl/Templater.hpp"
using support::Diagnostics;
using support::GlobalContext;
using support::Source;
using table::Driver;
using tpl::Templater;

auto main() -> int
{
    try {
        GlobalContext ctx;
        Diagnostics diag(std::cout);

        auto symbols = Driver(&ctx, &diag).compile("../samples/Simple.tbl");

        Source* src = ctx.load("../samples/Simple.md.tpl");
        std::cout << Templater(src, symbols.get());

        return diag.hasErrors() ? EXIT_FAILURE : EXIT_SUCCESS;
    } catch (std::exception& exc) {
        std::cerr << exc.what();
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_FAILURE;
    }
}

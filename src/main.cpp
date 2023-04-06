//
// Created by Albert on 04/03/2023.
//
#include "Support/Diagnostics.hpp"
#include "Support/GlobalContext.hpp"
#include "Support/Source.hpp"
#include "Table/Driver.hpp"
#include "Table/SymbolTable.hpp"
#include "Tpl/Templater.hpp"
using support::GlobalContext;
using support::Source;
using table::Driver;
using tpl::Templater;

auto main() -> int
try {
    GlobalContext ctx {};

    auto symbols = Driver(&ctx).compile("../samples/Simple.tbl");
    Source* src = ctx.load("../samples/Simple.md.tpl");
    std::cout << Templater(&ctx, src, symbols.get());

    return EXIT_SUCCESS;
} catch (support::DiagException&) {
    return EXIT_FAILURE;
} catch (std::exception& exc) {
    std::cerr << exc.what() << std::endl;
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "Unknown error";
    return EXIT_FAILURE;
}

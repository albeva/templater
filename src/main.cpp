//
// Created by Albert on 04/03/2023.
//
#include "Table/Ast/Ast.hpp"
#include "Table/Driver.hpp"
using table::Driver;

auto main() -> int
{
    try {
        Driver driver {};
        driver.printAst("../samples/Simple.tbl");
        return driver.exitCode();
    } catch (std::exception& exc) {
        std::cerr << exc.what();
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_FAILURE;
    }
}
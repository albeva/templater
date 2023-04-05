# Library
add_library(
    templater_lib
    Support/Containers.hpp
    Support/Diagnostics.cpp
    Support/Diagnostics.hpp
    Support/GlobalContext.cpp
    Support/GlobalContext.hpp
    Support/GridLayout.cpp
    Support/GridLayout.hpp
    Support/LexerBase.hpp
    Support/MemoryPool.hpp
    Support/Separator.hpp
    Support/Source.cpp
    Support/Source.hpp
    Support/SourceLoc.hpp
    Support/TokenBase.hpp
    Support/Utils.hpp
    Support/VisitorMixin.hpp
    Table/Ast/Ast.hpp
    Table/Ast/Context.cpp
    Table/Ast/Context.hpp
    Table/Ast/Printer.cpp
    Table/Ast/Printer.hpp
    Table/Column.hpp
    Table/Driver.cpp
    Table/Driver.hpp
    Table/Gen/Generator.cpp
    Table/Gen/Generator.hpp
    Table/Parse/Lexer.cpp
    Table/Parse/Lexer.hpp
    Table/Parse/Parser.cpp
    Table/Parse/Parser.hpp
    Table/Parse/Token.cpp
    Table/Parse/Token.hpp
    Table/Printer.cpp
    Table/Printer.hpp
    Table/Symbol.cpp
    Table/Symbol.cpp
    Table/Symbol.hpp
    Table/SymbolTable.cpp
    Table/SymbolTable.hpp
    Table/Table.cpp
    Table/Table.hpp
    Table/Value.hpp
    Tpl/Lexer.cpp
    Tpl/Lexer.hpp
    Tpl/Templater.cpp
    Tpl/Templater.hpp
    Tpl/Token.cpp
    Tpl/Token.hpp)

# fmt
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 9.1.0)
FetchContent_MakeAvailable(fmt)
target_disable_static_analysis(fmt)

# lib options
target_link_libraries(
    templater_lib
    PUBLIC fmt::fmt
    PRIVATE project_options project_warnings)
target_include_directories(
    templater_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
target_precompile_headers(templater_lib PRIVATE pch.hpp)

# main executable
add_executable(templater main.cpp)
target_link_libraries(templater PRIVATE templater_lib project_options project_warnings)
target_precompile_headers(templater REUSE_FROM templater_lib)
set_target_properties(templater PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

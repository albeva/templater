# Main executable
add_library(
    templater_lib
    pch.hpp
    Support/Containers.hpp
    Support/Context.cpp
    Support/Context.hpp
    Support/Diagnostics.cpp
    Support/Diagnostics.hpp
    Support/Source.cpp
    Support/Source.hpp
    Support/SourceLoc.hpp
    Support/Utils.hpp
    Table/Ast.cpp
    Table/Ast.hpp
    Table/AstAllocator.hpp
    Table/Column.cpp
    Table/Column.hpp
    Table/Gen/TableValue.cpp
    Table/Gen/TableValue.hpp
    Table/Generator.cpp
    Table/Generator.hpp
    Table/Lexer.cpp
    Table/Lexer.hpp
    Table/Parser.cpp
    Table/Parser.hpp
    Table/Printer.cpp
    Table/Printer.hpp
    Table/Symbol.cpp
    Table/Symbol.hpp
    Table/SymbolTable.cpp
    Table/SymbolTable.hpp
    Table/Table.cpp
    Table/Table.hpp
    Table/Token.cpp
    Table/Token.hpp
    Table/Value.cpp
    Table/Value.hpp)

# fmt
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 9.1.0)
FetchContent_MakeAvailable(fmt)

# lib options
target_link_libraries(templater_lib PUBLIC fmt::fmt)
target_link_libraries(templater_lib PRIVATE project_options project_warnings)
target_precompile_headers(templater_lib PRIVATE pch.hpp)
target_include_directories(templater_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(templater_lib PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

# lbc executable
add_executable(templater main.cpp Table/Gen/TableValue.cpp Table/Gen/TableValue.hpp)
target_link_libraries(templater PRIVATE templater_lib project_options project_warnings)
set_target_properties(templater PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# avoid cppcheck errors from 3rd party
target_disable_static_analysis(fmt)

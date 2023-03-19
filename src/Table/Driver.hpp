//
// Created by Albert on 17/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast/Ast.hpp"

namespace support {
class Diagnostics;
class GlobalContext;
class Source;
}

namespace table {
namespace ast {
    class Context;
}
class SymbolTable;

class Driver final {
public:
    NO_COPY_AND_MOVE(Driver)
    Driver(support::GlobalContext* ctx, support::Diagnostics* diag);
    ~Driver();

    [[nodiscard]] auto parse(const std::filesystem::path& path) -> std::unique_ptr<ast::Context>;
    [[nodiscard]] auto compile(const std::filesystem::path& path) -> std::unique_ptr<SymbolTable>;

    void printAst(const std::filesystem::path& path);
    void printTable(const std::filesystem::path& path);

private:
    support::GlobalContext* m_context;
    support::Diagnostics* m_diag;
};

} // namespace table
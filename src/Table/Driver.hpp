//
// Created by Albert on 17/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast/Ast.hpp"
#include "Support/Context.hpp"

namespace support {
class Diagnostics;
class Context;
class Source;
}

namespace table {
class SymbolTable;

class Driver final {
public:
    NO_COPY_AND_MOVE(Driver)
    Driver();
    ~Driver();

    [[nodiscard]] auto parse(const std::filesystem::path& path) -> ast::Node<ast::Content>;
    [[nodiscard]] auto compile(const std::filesystem::path& path) -> support::Context::UniquePtr<SymbolTable>;

    void printAst(const std::filesystem::path& path);
    void printTable(const std::filesystem::path& path);

    [[nodiscard]] auto exitCode() const noexcept -> int;

private:
    std::unique_ptr<support::Context> m_context;
    std::unique_ptr<support::Diagnostics> m_diag;
};

} // namespace table
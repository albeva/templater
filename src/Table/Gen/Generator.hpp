//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/VisitorMixin.hpp"
#include "Table/Ast/Ast.hpp"

namespace support {
class Context;
class Diagnostics;
class Source;
}

namespace table {
namespace parser {
    struct Token;
}
class SymbolTable;
class Table;
}

namespace table::gen {

class GeneratorException final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Generator final {
public:
    NO_COPY_AND_MOVE(Generator)
    Generator(support::Context* ctx, support::Diagnostics* diag, support::Source* source, const ast::Content* node);
    ~Generator() = default;

    VISITOR_MIXIN
    [[nodiscard]] auto getSymbolTable() const noexcept { return m_symbolTable; }

private:
    void visit(const ast::Content* node);
    void visit(const ast::Import* node);
    void visit(const ast::Table* node);
    void visit(const ast::TableColumn* node);
    void visit(const ast::TableInherit* node);
    void visit(const ast::TableBody* node);
    void visit(const ast::TableRow* node);
    void visit(const ast::Member* node);

    [[noreturn]] void redefinition(const Identifier& id, support::SourceLoc existing) const;

    support::Context* m_ctx;
    support::Diagnostics* m_diag;
    support::Source* m_source;
    SymbolTable* m_symbolTable;
    Table* m_table = nullptr;
    size_t m_rowIndex = 0;
};

} // namespace table::gen

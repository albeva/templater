//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Support/Context.hpp"
#include "Support/VisitorMixin.hpp"
#include "Table/Ast/Ast.hpp"

namespace support {
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
    Generator(support::Context* ctx, support::Diagnostics* diag);
    ~Generator();

    auto visit(const ast::Content* node) -> support::Context::UniquePtr<SymbolTable>;

    VISITOR_MIXIN

private:
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
    support::Context::UniquePtr<SymbolTable> m_symbolTable;
    support::Context::UniquePtr<Table> m_table;
    size_t m_rowIndex;
};

} // namespace table::gen

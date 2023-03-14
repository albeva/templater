//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"

namespace templater {
class Context;
class Diagnostics;
class Source;
}

namespace templater::table {
class SymbolTable;
class Table;

class GeneratorException final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Generator final {
public:
    NO_COPY_AND_MOVE(Generator)
    Generator(Context* ctx, Diagnostics* diag, Source* source, const ast::Content* node);
    ~Generator() = default;

    void visit(const ast::Content* node);
    void visit(const ast::Import* node);
    void visit(const ast::Table* node);
    void visit(const ast::TableColumn* node);
    void visit(const ast::TableInherit* node);
    void visit(const ast::TableBody* node);
    void visit(const ast::TableRow* node);
    void visit(const ast::UnaryExpression* node);
    void visit(const ast::BinaryExpression* node);
    void visit(const ast::Member* node);

    [[nodiscard]] auto getSymbolTable() const { return m_symbolTable; }

    // for visiting with std::visit
    inline auto operator()(const auto* node) { return visit(node); }

    // visit with std::visit
    template <typename... Ts>
    inline auto visit(const std::variant<Ts...>& node) { return std::visit(*this, node); }

private:
    void inline visitEach(const std::ranges::range auto& list)
    {
        for (const auto& node : list) {
            visit(node);
        }
    }

    [[noreturn]] void redefinition(const Token& id, SourceLoc existing) const;

    Context* m_ctx;
    Diagnostics* m_diag;
    Source* m_source;
    SymbolTable* m_symbolTable;
    Table* m_table = nullptr;
    size_t m_rowIndex = 0;
    size_t m_colIndex = 0;
};

} // namespace templater::table

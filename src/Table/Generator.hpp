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

class Generator final {
public:
    NO_COPY_AND_MOVE(Generator)
    Generator(Context* ctx, Diagnostics* diag, Source* source, const ast::Content* node);
    ~Generator() = default;

    void operator()(const ast::Content* node);
    void operator()(const ast::Import* node);
    void operator()(const ast::Table* node);
    void operator()(const ast::TableColumn* node);
    void operator()(const ast::TableInherit* node);
    void operator()(const ast::TableBody* node);
    void operator()(const ast::TableRow* node);
    void operator()(const ast::StructBody* node);
    void operator()(const ast::UnaryExpression* node);
    void operator()(const ast::BinaryExpression* node);
    void operator()(const ast::Literal* node);
    void operator()(const ast::Member* node);

    void inline visit(const auto* node) { operator()(node); } // cppcheck-suppress functionStatic

    template <typename... Ts>
    void inline visit(const std::variant<Ts...>& node) { std::visit(*this, node); }

    void inline visitEach(const std::ranges::range auto& list)
    {
        for (const auto& node : list) {
            visit(node);
        }
    }

private:
    Context* m_ctx;
    Diagnostics* m_diag;
    Source* m_source;
    SymbolTable* m_symbolTable;

    Table* m_table = nullptr;
};

} // namespace templater::table

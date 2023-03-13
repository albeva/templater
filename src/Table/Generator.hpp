//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"

namespace templater {
class Context;
}

namespace templater::table {
class SymbolTable;

class Generator final {
public:
    NO_COPY_AND_MOVE(Generator)
    Generator(Context* ctx, const ast::Content* node);
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

    template <typename T>
    void visit(const T* node) { operator()(node); } // cppcheck-suppress functionStatic

    template <typename... Ts>
    void visit(const std::variant<Ts...>& node) { std::visit(*this, node); }

private:
    Context* m_ctx;
    SymbolTable* m_symbolTable;
};

} // namespace templater::table

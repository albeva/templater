//
// Created by Albert on 05/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"

namespace templater::table::ast {

class Printer final {
public:
    NO_COPY_AND_MOVE(Printer)
    Printer() = default;
    explicit Printer(const Content* node) { visit(node); }
    ~Printer() = default;

    void operator()(const Content* node);
    void operator()(const Import* node);
    void operator()(const Table* node);
    void operator()(const TableColumn* node);
    void operator()(const TableInherit* node);
    void operator()(const TableBody* node);
    void operator()(const TableRow* node);
    void operator()(const TableValue& node);
    void operator()(const StructBody* node);
    void operator()(const UnaryExpression* node);
    void operator()(const BinaryExpression* node);
    void operator()(const Literal* node);
    void operator()(const Member* node);

    [[nodiscard]] auto output() const -> std::string;

    template <typename T>
    void visit(const T* node) { operator()(node); } // cppcheck-suppress functionStatic

    template <typename... Ts>
    void visit(const std::variant<Ts...>& node) { std::visit(*this, node); }

private:
    std::size_t m_indent = 0;
    std::stringstream m_output {};
    [[nodiscard]] auto spaces() const { return std::string(m_indent * 4, ' '); }
};

auto inline operator<<(std::ostream& os, const Printer& printer) -> std::ostream&
{
    os << printer.output();
    return os;
}

} // namespace templater::table::ast

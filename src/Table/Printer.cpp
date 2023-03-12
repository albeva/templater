//
// Created by Albert on 05/03/2023.
//
#include "Printer.hpp"
#include "Ast.hpp"
#include "Token.hpp"

using namespace templater::table::ast;

void Printer::visit(StatementList& node)
{
    for (const auto& stmt : node.statements) {
        visit(*stmt);
        std::cout << "\n";
    }
}

void Printer::visit(Import& node)
{
    std::cout << spaces() << "import \"" << node.file << "\" as " << node.identifier;
}

void Printer::visit(Table& node)
{
    std::cout << '\n'
              << spaces() << "table " << node.identifier;
    if (!node.columns.empty()) {
        std::cout << "(";
        bool isFirst = true;
        for (const auto& col : node.columns) {
            if (isFirst) {
                isFirst = false;
            } else {
                std::cout << ' ';
            }
            visit(*col);
        }
        std::cout << ")";
    }

    if (!node.content.empty()) {
        std::cout << " = ";
        bool isFirst = true;
        for (const auto& content : node.content) {
            if (isFirst) {
                isFirst = false;
            } else {
                std::cout << " + ";
            }
            visit(*content);
        }
    }
}

void Printer::visit(TableColumn& node)
{
    std::cout << node.identifier;
    if (auto* value = node.value) {
        std::cout << " = ";
        visit(*value);
    }
}

void Printer::visit(TableInherit& node)
{
    visit(*node.member);
    if (auto* expr = node.expression) {
        std::cout << "(";
        visit(*expr);
        std::cout << ")";
    }
}

void Printer::visit(TableBody& node)
{
    std::cout << "[";
    m_indent++;
    for (const auto& row : node.rows) {
        std::cout << '\n'
                  << spaces();
        visit(*row);
    }
    m_indent--;
    std::cout << "\n"
              << spaces() << "]";
}

void Printer::visit(TableRow& node)
{
    bool isFirst = true;
    for (const auto& value : node.values) {
        if (isFirst) {
            isFirst = false;
        } else {
            std::cout << ' ';
        }
        visit(*value);
    }
}

void Printer::visit(TableValue& node)
{
    std::visit(
        templater::Visitor {
            [](const Literal* node) {
                if (node->type == TokenKind::String) {
                    std::cout << '"' << node->value << '"';
                } else {
                    std::cout << node->value;
                }
            },
            [&](StructBody* node) {
                visit(*node);
            },
        },
        node.value);
}

void Printer::visit(StructBody& node)
{
    (void)node;
    std::cout << "{}";
}

void Printer::visit(UnaryExpression& node)
{
    std::cout << Token::describe(node.type) << '(';
    visit(*node.rhs);
    std::cout << ')';
}

void Printer::visit(BinaryExpression& node)
{
    std::cout << '(';
    visit(*node.lhs);
    std::cout << ' ' << Token::describe(node.type) << ' ';
    visit(*node.rhs);
    std::cout << ')';
}

void Printer::visit(Literal& node)
{
    if (node.type == TokenKind::String) {
        std::cout << '"' << node.value << '"';
    } else {
        std::cout << node.value;
    }
}

void Printer::visit(Member& node)
{
    bool isFirst = true;
    for (const auto& id : node.identifiers) {
        if (isFirst) {
            isFirst = false;
        } else {
            std::cout << '.';
        }
        std::cout << id;
    }
}

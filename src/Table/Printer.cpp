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
        m_output << "\n";
    }
}

void Printer::visit(Import& node)
{
    m_output << spaces() << "import \"" << node.file << "\" as " << node.identifier;
}

void Printer::visit(Table& node)
{
    m_output << '\n'
             << spaces() << "table " << node.identifier;
    if (!node.columns.empty()) {
        m_output << "(";
        bool isFirst = true;
        for (const auto& col : node.columns) {
            if (isFirst) {
                isFirst = false;
            } else {
                m_output << ' ';
            }
            visit(*col);
        }
        m_output << ")";
    }

    if (!node.content.empty()) {
        m_output << " = ";
        bool isFirst = true;
        for (const auto& content : node.content) {
            if (isFirst) {
                isFirst = false;
            } else {
                m_output << " + ";
            }
            visit(*content);
        }
    }
}

void Printer::visit(TableColumn& node)
{
    m_output << node.identifier;
    if (auto* value = node.value) {
        m_output << " = ";
        visit(*value);
    }
}

void Printer::visit(TableInherit& node)
{
    visit(*node.member);
    if (auto* expr = node.expression) {
        m_output << "(";
        visit(*expr);
        m_output << ")";
    }
}

void Printer::visit(TableBody& node)
{
    m_output << "[";
    m_indent++;
    for (const auto& row : node.rows) {
        m_output << '\n'
                 << spaces();
        visit(*row);
    }
    m_indent--;
    m_output << "\n"
             << spaces() << "]";
}

void Printer::visit(TableRow& node)
{
    bool isFirst = true;
    for (const auto& value : node.values) {
        if (isFirst) {
            isFirst = false;
        } else {
            m_output << ' ';
        }
        visit(*value);
    }
}

void Printer::visit(TableValue& node)
{
    std::visit(
        templater::Visitor {
            [&](const Literal* node) {
                if (node->type == TokenKind::String) {
                    m_output << '"' << node->value << '"';
                } else {
                    m_output << node->value;
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
    m_output << "{}";
}

void Printer::visit(UnaryExpression& node)
{
    m_output << Token::describe(node.type) << '(';
    visit(*node.rhs);
    m_output << ')';
}

void Printer::visit(BinaryExpression& node)
{
    m_output << '(';
    visit(*node.lhs);
    m_output << ' ' << Token::describe(node.type) << ' ';
    visit(*node.rhs);
    m_output << ')';
}

void Printer::visit(Literal& node)
{
    if (node.type == TokenKind::String) {
        m_output << '"' << node.value << '"';
    } else {
        m_output << node.value;
    }
}

void Printer::visit(Member& node)
{
    bool isFirst = true;
    for (const auto& id : node.identifiers) {
        if (isFirst) {
            isFirst = false;
        } else {
            m_output << '.';
        }
        m_output << id;
    }
}

auto Printer::output() const -> std::string
{
    return m_output.str();
}

//
// Created by Albert on 05/03/2023.
//
#include "Printer.hpp"
#include "Ast.hpp"
#include "Token.hpp"

using namespace templater::table::ast;

void Printer::visit(Content& node)
{
    for (const auto& stmt : node.getStatements()) {
        visit(*stmt);
        m_output << "\n";
    }
}

void Printer::visit(Import& node)
{
    m_output << spaces() << "import \"" << node.getFile() << "\" as " << node.getIdentifier();
}

void Printer::visit(Table& node)
{
    m_output << '\n'
             << spaces() << "table " << node.getIdentifier();
    if (!node.getColumns().empty()) {
        m_output << "(";
        bool isFirst = true;
        for (const auto& col : node.getColumns()) {
            if (isFirst) {
                isFirst = false;
            } else {
                m_output << ' ';
            }
            visit(*col);
        }
        m_output << ")";
    }

    if (!node.getContent().empty()) {
        m_output << " = ";
        bool isFirst = true;
        for (const auto& content : node.getContent()) {
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
    m_output << node.getIdentifier();
    if (auto* value = node.getValue()) {
        m_output << " = ";
        visit(*value);
    }
}

void Printer::visit(TableInherit& node)
{
    visit(*node.getMember());
    if (auto* expr = node.getExpression()) {
        m_output << "(";
        visit(*expr);
        m_output << ")";
    }
}

void Printer::visit(TableBody& node)
{
    m_output << "[";
    m_indent++;
    for (const auto& row : node.getRows()) {
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
    for (const auto& value : node.getValues()) {
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
            [&](const Literal* lit) {
                if (lit->getType() == TokenKind::String) {
                    m_output << '"' << lit->getValue() << '"';
                } else {
                    m_output << lit->getValue();
                }
            },
            [&](StructBody* body) {
                visit(*body);
            },
        },
        node.getValue());
}

void Printer::visit(StructBody& node)
{
    (void)node;
    m_output << "{}";
}

void Printer::visit(UnaryExpression& node)
{
    m_output << Token::describe(node.getType()) << '(';
    visit(*node.getRhs());
    m_output << ')';
}

void Printer::visit(BinaryExpression& node)
{
    m_output << '(';
    visit(*node.getLhs());
    m_output << ' ' << Token::describe(node.getType()) << ' ';
    visit(*node.getRhs());
    m_output << ')';
}

void Printer::visit(Literal& node)
{
    if (node.getType() == TokenKind::String) {
        m_output << '"' << node.getValue() << '"';
    } else {
        m_output << node.getValue();
    }
}

void Printer::visit(Member& node)
{
    bool isFirst = true;
    for (const auto& id : node.getIdentifiers()) {
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

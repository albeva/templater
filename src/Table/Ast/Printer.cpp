//
// Created by Albert on 05/03/2023.
//
#include "Printer.hpp"
#include "Ast.hpp"
#include "Table/Parse/Token.hpp"
using namespace templater::table::ast;
using namespace templater::table::parser;

void Printer::visit(const Content* node)
{
    for (const auto& stmt : node->getStatements()) {
        visit(stmt);
        m_output << "\n";
    }
}

void Printer::visit(const Import* node)
{
    m_output << spaces() << "import \"" << node->getFile() << "\" as " << node->getIdentifier();
}

void Printer::visit(const Table* node)
{
    m_output << '\n'
             << spaces() << "table " << node->getIdentifier().getValue();
    if (!node->getColumns().empty()) {
        m_output << "(";
        bool isFirst = true;
        for (const auto* col : node->getColumns()) {
            if (isFirst) {
                isFirst = false;
            } else {
                m_output << ' ';
            }
            visit(col);
        }
        m_output << ")";
    }

    if (!node->getContent().empty()) {
        m_output << " = ";
        bool isFirst = true;
        for (const auto& content : node->getContent()) {
            if (isFirst) {
                isFirst = false;
            } else {
                m_output << " + ";
            }
            visit(content);
        }
    }
}

void Printer::visit(const TableColumn* node)
{
    m_output << node->getIdentifier().getValue();
    if (auto value = node->getValue()) {
        m_output << " = ";
        visit(value.value());
    }
}

void Printer::visit(const TableInherit* node)
{
    visit(node->getMember());
    if (auto expr = node->getExpression()) {
        visit(expr.value());
    }
}

void Printer::visit(const TableBody* node)
{
    m_output << "[";
    m_indent++;
    for (const auto* row : node->getRows()) {
        m_output << '\n'
                 << spaces();
        visit(row);
    }
    m_indent--;
    m_output << "\n"
             << spaces() << "]";
}

void Printer::visit(const TableRow* node)
{
    bool isFirst = true;
    for (const auto value : node->getValues()) {
        if (isFirst) {
            isFirst = false;
        } else {
            m_output << ' ';
        }
        visit(value);
    }
}

void Printer::visit(const StructBody* node)
{
    (void)node;
    m_output << "{}";
}

// NOLINTNEXTLINE misc-no-recursion
void Printer::visit(const UnaryExpression* node)
{
    m_output << Token::describe(node->getType()) << '(';
    visit(node->getRhs());
    m_output << ')';
}

// NOLINTNEXTLINE misc-no-recursion
void Printer::visit(const BinaryExpression* node)
{
    m_output << '(';
    visit(node->getLhs());
    m_output << ' ' << Token::describe(node->getType()) << ' ';
    visit(node->getRhs());
    m_output << ')';
}

void Printer::visit(const Token& token)
{
    if (token.getKind() == TokenKind::String) {
        m_output << '"' << token.getValue() << '"';
    } else {
        m_output << token.getValue();
    }
}

void Printer::visit(const Member* node)
{
    bool isFirst = true;
    for (const auto& id : node->getIdentifiers()) {
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
//
// Created by Albert on 05/03/2023.
//
#include "Printer.hpp"
#include "Ast.hpp"
#include "Token.hpp"

using namespace templater::table::ast;

void Printer::operator()(const Content* node)
{
    for (const auto& stmt : node->getStatements()) {
        visit(stmt);
        m_output << "\n";
    }
}

void Printer::operator()(const Import* node)
{
    m_output << spaces() << "import \"" << node->getFile() << "\" as " << node->getIdentifier();
}

void Printer::operator()(const Table* node)
{
    m_output << '\n'
             << spaces() << "table " << node->getIdentifier();
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

void Printer::operator()(const TableColumn* node)
{
    m_output << node->getIdentifier();
    if (auto value = node->getValue()) {
        m_output << " = ";
        visit(value.value());
    }
}

void Printer::operator()(const TableInherit* node)
{
    visit(node->getMember());
    if (auto expr = node->getExpression()) {
        visit(expr.value());
    }
}

void Printer::operator()(const TableBody* node)
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

void Printer::operator()(const TableRow* node)
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

void Printer::operator()(const TableValue& node)
{
    visit(node);
}

void Printer::operator()(const StructBody* node)
{
    (void)node;
    m_output << "{}";
}

// NOLINTNEXTLINE misc-no-recursion
void Printer::operator()(const UnaryExpression* node)
{
    m_output << Token::describe(node->getType()) << '(';
    visit(node->getRhs());
    m_output << ')';
}

// NOLINTNEXTLINE misc-no-recursion
void Printer::operator()(const BinaryExpression* node)
{
    m_output << '(';
    visit(node->getLhs());
    m_output << ' ' << Token::describe(node->getType()) << ' ';
    visit(node->getRhs());
    m_output << ')';
}

void Printer::operator()(const Literal* node)
{
    if (node->getType() == TokenKind::String) {
        m_output << '"' << node->getValue() << '"';
    } else {
        m_output << node->getValue();
    }
}

void Printer::operator()(const Member* node)
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

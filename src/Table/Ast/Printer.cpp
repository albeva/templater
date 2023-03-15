//
// Created by Albert on 05/03/2023.
//
#include "Printer.hpp"
#include "Ast.hpp"
#include "Support/Separator.hpp"
#include "Table/Parse/Token.hpp"
using templater::support::Separator;
using templater::table::ast::Printer;
using templater::table::parser::Token;
using templater::table::parser::TokenKind;

void Printer::visit(const Content* node)
{
    for (const auto& stmt : node->getStatements()) {
        visit(stmt);
        m_output << "\n";
    }
}

void Printer::visit(const Import* node)
{
    fmt::print(m_output,
        "{0:<{1}}import \"{2}\" as {3}",
        "", (m_indent * 4),
        node->getFile().getValue(),
        node->getIdentifier().getValue());
}

void Printer::visit(const Table* node)
{
    m_output << '\n'
             << spaces() << "table " << node->getIdentifier().getValue();
    if (!node->getColumns().empty()) {
        m_output << "(";
        Separator sep { " " };
        for (const auto* col : node->getColumns()) {
            m_output << sep();
            visit(col);
        }
        m_output << ")";
    }

    if (!node->getContent().empty()) {
        m_output << " = ";
        Separator sep { " + " };
        for (const auto& content : node->getContent()) {
            m_output << sep();
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
    Separator sep { " " };
    for (const auto value : node->getValues()) {
        m_output << sep();
        if (value.has_value()) {
            visit(value.value());
        } else {
            m_output << '-';
        }
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
    m_output << Token::describe(node->getOp().getKind()) << '(';
    visit(node->getRhs());
    m_output << ')';
}

// NOLINTNEXTLINE misc-no-recursion
void Printer::visit(const BinaryExpression* node)
{
    m_output << '(';
    visit(node->getLhs());
    m_output << ' ' << Token::describe(node->getOp().getKind()) << ' ';
    visit(node->getRhs());
    m_output << ')';
}

void Printer::visit(const Member* node)
{
    Separator sep { "." };
    for (const auto& id : node->getIdentifiers()) {
        m_output << sep() << id.getValue();
    }
}

void Printer::visit(const PipeLiteral& /*pipe*/)
{
    m_output << '|';
}

void Printer::visit(const Identifier& node)
{
    m_output << node.getValue();
}

void Printer::visit(const StringLiteral& node)
{
    m_output << '"' << std::quoted(node.getValue()) << '"';
}

void Printer::visit(const NumberLiteral& node)
{
    m_output << node.getValue();
}

auto Printer::output() const -> std::string
{
    return m_output.str();
}

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
        fmt::print(m_output, "\n");
    }
}

void Printer::visit(const Import* node)
{
    fmt::print(m_output,
        "{0:<{1}}import \"{2}\" as {3}",
        "", m_indent,
        node->getFile().getValue(),
        node->getIdentifier().getValue());
}

void Printer::visit(const Table* node)
{
    fmt::print(m_output, "{0:<}{1}{2}", "", m_indent, node->getIdentifier().getValue());
    if (!node->getColumns().empty()) {
        fmt::print(m_output, "(");
        Separator sep { " " };
        for (const auto* col : node->getColumns()) {
            fmt::print(m_output, "{}", sep());
            visit(col);
        }
        fmt::print(m_output, ")");
    }

    if (!node->getContent().empty()) {
        fmt::print(m_output, " = ");
        Separator sep { " + " };
        for (const auto& content : node->getContent()) {
            fmt::print(m_output, "{}", sep());
            visit(content);
        }
    }
}

void Printer::visit(const TableColumn* node)
{
    m_output << node->getIdentifier().getValue();
    if (auto value = node->getValue()) {
        fmt::print(m_output, " = ");
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
    fmt::print(m_output, "[");
    m_indent += 4;
    for (const auto* row : node->getRows()) {
        fmt::print(m_output, "\n{0:<{1}}", "", m_indent);
        visit(row);
    }
    m_indent -= 4;
    fmt::print(m_output, "\n{0:<{1}}]", "", m_indent);
}

void Printer::visit(const TableRow* node)
{
    Separator sep { " " };
    for (const auto value : node->getValues()) {
        fmt::print(m_output, "{}", sep());
        if (value.has_value()) {
            visit(value.value());
        } else {
            fmt::print(m_output, "-");
        }
    }
}

// NOLINTNEXTLINE misc-no-recursion
void Printer::visit(const UnaryExpression* node)
{
    fmt::print(m_output, "{}", Token::describe(node->getOp().getKind()));
    visit(node->getRhs());
}

// NOLINTNEXTLINE misc-no-recursion
void Printer::visit(const BinaryExpression* node)
{
    visit(node->getLhs());
    fmt::print(m_output, " {} ", Token::describe(node->getOp().getKind()));
    visit(node->getRhs());
}

void Printer::visit(const Member* node)
{
    Separator sep { "." };
    for (const auto& id : node->getIdentifiers()) {
        fmt::print(m_output, "{}{}", sep(), id.getValue());
    }
}

void Printer::visit(const PipeLiteral& /*pipe*/)
{
    fmt::print(m_output, "{}", '|');
}

void Printer::visit(const Identifier& node)
{
    fmt::print(m_output, "{}", node.getValue());
}

void Printer::visit(const StringLiteral& node)
{
    m_output << std::quoted(node.getValue());
}

void Printer::visit(const NumberLiteral& node)
{
    fmt::print(m_output, "{}", node.getValue());
}

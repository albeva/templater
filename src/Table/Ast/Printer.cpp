//
// Created by Albert on 05/03/2023.
//
#include "Printer.hpp"
#include "Ast.hpp"
#include "Support/GridLayout.hpp"
#include "Support/Separator.hpp"
#include "Table/Parse/Token.hpp"
using support::printGrid;
using support::Separator;
using table::toString;
using table::ast::Printer;
using table::parser::Token;
using table::parser::TokenKind;

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
        "import \"{}\" as {}",
        node->getFile().getValue(),
        node->getIdentifier().getValue());
}

void Printer::visit(const Table* node)
{
    fmt::print(m_output, "table {}", node->getIdentifier().getValue());
    if (!node->getColumns().empty()) {
        fmt::print(m_output, "(");
        Separator sep { " " };
        for (const auto& col : node->getColumns()) {
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
        fmt::print(m_output, " = {}", toString(value.value()));
    }
}

void Printer::visit(const TableInherit* node)
{
    visit(node->getMember());
    if (const auto& expr = node->getExpression()) {
        visit(expr.value());
    }
}

void Printer::visit(const TableBody* node)
{
    fmt::print(m_output, "[");
    std::vector<std::vector<std::string>> content {};
    content.reserve(node->getRows().size());
    for (const auto& row : node->getRows()) {
        visit(content.emplace_back(), row);
    }
    printGrid(m_output, content);
    fmt::print(m_output, "\n]");
}

void Printer::visit(std::vector<std::string>& dst, const TableRow* node)
{
    dst.reserve(node->getValues().size());
    std::ranges::transform(node->getValues(), std::back_inserter(dst), [](const TableValue& value) {
        return std::visit(
            support::Visitor {
                [](const std::monostate&) {
                    return std::string("-");
                },
                [](const PipeLiteral&) {
                    return std::string("|");
                },
                [](const Value& val) {
                    return toString(val);
                },
            },
            value);
    });
}

auto Printer::visit(const Value& value) -> std::string
{
    return toString(value);
}

// NOLINTNEXTLINE misc-no-recursion
auto Printer::visit(const UnaryExpression* node) -> std::string
{
    return fmt::format("{} {}",
        Token::describe(node->getOp().getKind()),
        visit(node->getRhs()));
}

// NOLINTNEXTLINE misc-no-recursion
auto Printer::visit(const BinaryExpression* node) -> std::string
{
    return fmt::format("{} {} {}",
        visit(node->getLhs()),
        Token::describe(node->getOp().getKind()),
        visit(node->getRhs()));
}

auto Printer::visit(const Member* node) -> std::string
{
    std::stringstream ss;
    Separator sep { "." };
    for (const auto& id : node->getIdentifiers()) {
        fmt::print(ss, "{}{}", sep(), id.getValue());
    }
    return ss.str();
}

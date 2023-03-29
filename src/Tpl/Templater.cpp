//
// Created by Albert on 19/03/2023.
//
#include "Templater.hpp"
#include "Lexer.hpp"
#include "Support/GlobalContext.hpp"
#include "Support/Source.hpp"
using tpl::Templater;

Templater::Templater(support::GlobalContext* ctx, support::Source* source, table::SymbolTable* symbolTable)
    : m_ctx(ctx)
    , m_source(source)
    , m_symbolTable(symbolTable)
{
    (void)m_symbolTable;
    collectTokens();
}

Templater::~Templater() = default;

void Templater::collectTokens()
{
    Lexer lexer(m_ctx, m_source->source());
    Token tkn {};
    do {
        lexer.next(tkn);
        m_tokens.emplace_back(tkn);
        if (tkn.is(TokenKind::Invalid)) {
            break;
        }
    } while (tkn.isNot(TokenKind::EndOfFile));
}

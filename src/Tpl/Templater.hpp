//
// Created by Albert on 19/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Token.hpp"

namespace support {
class Source;
class GlobalContext;
}
namespace table {
class SymbolTable;
}

namespace tpl {

class Templater final {
public:
    NO_COPY_AND_MOVE(Templater)
    Templater(support::GlobalContext* ctx, support::Source* source, table::SymbolTable* symbolTable);
    ~Templater();

    [[nodiscard]] inline auto output() const { return m_output.str(); }

private:
    void collectTokens();

    support::GlobalContext* m_ctx;
    support::Source* m_source;
    table::SymbolTable* m_symbolTable;
    std::stringstream m_output;
    std::vector<Token> m_tokens;
};

auto inline operator<<(std::ostream& os, const Templater& tpl) -> std::ostream&
{
    os << tpl.output();
    return os;
}

} // namespace tpl

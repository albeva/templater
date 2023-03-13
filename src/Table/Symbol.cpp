//
// Created by Albert on 13/03/2023.
//
#include "Symbol.hpp"
using namespace templater::table;

auto Symbol::getKind() const -> SymbolKind
{
    static constexpr auto visitor = Visitor {
        [](const Table*) {
            return SymbolKind::Table;
        },
        [](const Column*) {
            return SymbolKind::Column;
        },
        [](const SymbolTable*) {
            return SymbolKind::Namespace;
        },
    };
    return std::visit(visitor, m_value);
}

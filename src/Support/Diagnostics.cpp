//
// Created by Albert on 14/03/2023.
//
#include "Diagnostics.hpp"
#include "Source.hpp"
#include "SourceLoc.hpp"
#include <fmt/ostream.h>
using namespace templater;

namespace {
[[nodiscard]] constexpr auto getString(Diagnostics::Level level) -> std::string_view
{
    switch (level) {
    case templater::Diagnostics::Level::Error:
        return "error"sv;
    case templater::Diagnostics::Level::Notice:
        return "note"sv;
    default:
        std::unreachable();
    }
}
}

Diagnostics::Diagnostics(std::ostream& output)
    : m_output(output)
    , m_errorCount(0)
{
}

void Diagnostics::print(Level level, const Source* source, const SourceLoc& loc, const std::string& message)
{
    if (level == Level::Error) {
        m_errorCount++;
    }

    auto pos = source->getPosition(loc);
    fmt::print(
        m_output,
        "{}({},{}): {}: {}\n"
        "{}\n",
        source->getName(), pos.getLine(), pos.getCol(), getString(level), message,
        source->highlight(pos));
}

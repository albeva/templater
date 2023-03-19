//
// Created by Albert on 12/03/2023.
//
#include "GlobalContext.hpp"
#include "Diagnostics.hpp"
#include "Source.hpp"
using support::GlobalContext;

GlobalContext::GlobalContext(std::ostream& output)
    : m_pool()
    , m_uniquedStrings(m_pool.getAllocator())
    , m_sources(m_pool.getAllocator())
    , m_diag(m_pool.makeUnique<Diagnostics>(output))
{
}

GlobalContext::~GlobalContext() = default;

auto GlobalContext::load(const std::filesystem::path& path) -> Source*
{
    auto res = std::ranges::find_if(m_sources, [&](const auto& existing) {
        return existing->getName() == path;
    });
    if (res != m_sources.end()) {
        return res->get();
    }

    return m_sources.emplace_back(m_pool.makeUnique<Source>(path)).get();
}

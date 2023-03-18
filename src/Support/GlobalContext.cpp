//
// Created by Albert on 12/03/2023.
//
#include "GlobalContext.hpp"
#include "Source.hpp"
using support::GlobalContext;

GlobalContext::GlobalContext()
    : m_pa(&m_mbr)
    , m_uniquedStrings(m_pa)
    , m_sources(m_pa)
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

    return m_sources.emplace_back(makeUnique<Source>(path)).get();
}

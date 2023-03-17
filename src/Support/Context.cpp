//
// Created by Albert on 12/03/2023.
//
#include "Context.hpp"
#include "Source.hpp"
using support::Context;

Context::Context()
    : m_pa(&m_mbr)
    , m_uniquedStrings(m_pa)
    , m_sources(m_pa)
{
}

Context::~Context() = default;

auto Context::load(const std::filesystem::path& path) -> Source*
{
    auto res = std::ranges::find_if(m_sources, [&](const auto& existing) {
        return existing->getName() == path;
    });
    if (res != m_sources.end()) {
        return res->get();
    }

    return m_sources.emplace_back(create<Source>(path)).get();
}

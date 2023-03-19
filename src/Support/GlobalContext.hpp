//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Containers.hpp"
#include "MemoryPool.hpp"
namespace support {
class Diagnostics;
class Source;

class GlobalContext final {
public:
    NO_COPY_AND_MOVE(GlobalContext)
    explicit GlobalContext(std::ostream& output = std::cerr);
    ~GlobalContext();

    [[nodiscard]] inline auto getPool() const noexcept -> auto& { return m_pool; }

    /**
     * Retain copy of given string and return unique string_view
     */
    [[nodiscard]] inline auto retain(std::string&& str) -> std::string_view
    {
        return *m_uniquedStrings.emplace(std::move(str)).first;
    }

    [[nodiscard]] inline auto retain(std::string_view str) -> std::string_view
    {
        return *m_uniquedStrings.emplace(str).first;
    }

    /**
     * Load Source with given path. Context retains ownership
     */
    [[nodiscard]] auto load(const std::filesystem::path& path) -> Source*;

    /**
     * Get global diagnostics object. It generates output to std::cerr
     */
    [[nodiscard]] inline auto getDiagnostics() const noexcept -> Diagnostics* { return m_diag.get(); }

private:
    MemoryPool m_pool;
    pmr::StringSet m_uniquedStrings;
    std::pmr::vector<MemoryPool::UniquePtr<Source>> m_sources;
    MemoryPool::UniquePtr<Diagnostics> m_diag;
};

} // namespace support

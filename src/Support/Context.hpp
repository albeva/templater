//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
namespace support {
class Source;

class Context final {
public:
    NO_COPY_AND_MOVE(Context)
    Context();
    ~Context();

    template <typename T>
    using UniquePtr = std::unique_ptr<T, support::pmr::MonotonicBufferDelete>;

    /**
     * Allocate memory for T and create T at allocated memory
     */
    template <typename T, typename... Args>
    [[nodiscard]] inline auto create(Args&&... args) -> T*
    {
        auto* res = m_pa.allocate_object<T>();
        return std::construct_at(res, std::forward<Args>(args)...);
    }

    /**
     * Similar to std::make_unique<T>, returns UniquePtr
     */
    template <typename T, typename... Args>
    [[nodiscard]] inline auto makeUnique(Args&&... args) -> UniquePtr<T>
    {
        return UniquePtr<T>(create<T>(std::forward<Args>(args)...));
    }

    /**
     * Get polymorphic_allocator instance
     */
    [[nodiscard]] inline auto getAllocator() const noexcept -> auto& { return m_pa; }

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

private:
    std::pmr::monotonic_buffer_resource m_mbr;
    std::pmr::polymorphic_allocator<std::byte> m_pa;
    pmr::StringSet m_uniquedStrings;
    std::pmr::vector<UniquePtr<Source>> m_sources;
};

} // namespace support

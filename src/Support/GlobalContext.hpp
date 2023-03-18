//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
namespace support {
class Source;

class GlobalContext final {
public:
    NO_COPY_AND_MOVE(GlobalContext)
    GlobalContext();
    ~GlobalContext();

    /**
     * Objects allocated in memory pool don't need to deallocate their memory,
     * but we still want the dtor to be called.
     */
    struct MonotonicBufferDelete {
        template <typename T>
        constexpr void operator()(T* obj) const { std::destroy_at(obj); }
    };
    template <typename T>
    using UniquePtr = std::unique_ptr<T, MonotonicBufferDelete>;

    /**
     * Allocate memory for T in the memory pool and create T. This returns
     * a raw pointer means no dtor will be called, unless manually called ~T()
     */
    template <typename T, typename... Args>
    [[nodiscard]] constexpr auto create(Args&&... args) -> T*
    {
        auto* mem = m_pa.allocate_object<T>();
        return std::construct_at(mem, std::forward<Args>(args)...);
    }

    /**
     * Allocate memory for T in the memory pool and create T
     *
     * The UniquePtr will cause dtor to be called as expected, but
     * memory will not be freed until Context itself is deallocated
     */
    template <typename T, typename... Args>
    [[nodiscard]] constexpr auto makeUnique(Args&&... args) -> UniquePtr<T>
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

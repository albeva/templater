//
// Created by Albert on 18/03/2023.
//
#pragma once
#include "pch.hpp"

namespace support {

/**
 * A memory pool allocator that uses C++20 pmr (polymorphic memory resource) features.
 * This memory pool is designed to provide efficient memory allocation for objects.
 */
struct MemoryPool final {
    NO_COPY_AND_MOVE(MemoryPool)
    MemoryPool() = default;

    /**
     * Constructor with upstream memory resource.
     * @param upstream An upstream memory resource for the monotonic buffer resource.
     */
    explicit MemoryPool(std::pmr::memory_resource* upstream)
        : m_mbr(upstream), m_pa(&m_mbr) {}

    ~MemoryPool() = default;

    /**
     * A custom deleter for the unique_ptr that destructs the object without deallocating memory.
     * This deleter is suitable for objctex created in the pool, as the memory will be deallocated by the pool itself.
     */
    struct MonotonicBufferDelete {
        template <typename T>
        constexpr void operator()(T* obj) const noexcept { std::destroy_at(obj); }
    };

    /**
     * A unique pointer with a custom deleter, designed for use with the MemoryPool.
     *
     * @tparam T The type of the object managed by the unique_ptr.
     */
    template <typename T>
    using UniquePtr = std::unique_ptr<T, MonotonicBufferDelete>;

    /**
     * Create a unique_ptr of an object of type T with the provided arguments in the memory pool.
     *
     * @tparam T The type of the object to be created.
     * @tparam Args The types of the arguments to be forwarded to the constructor of the object.
     * @param args The arguments to be forwarded to the constructor of the object.
     * @return A UniquePtr<T> pointing to the newly created object.
     */
    template <typename T, typename... Args>
    [[nodiscard]] constexpr auto makeUnique(Args&&... args) -> UniquePtr<T>
    {
        return UniquePtr<T>(create<T>(std::forward<Args>(args)...));
    }

    /**
     * Create an object of type T with the provided arguments in the memory pool.
     *
     * @tparam T The type of the object to be created.
     * @tparam Args The types of the arguments to be forwarded to the constructor of the object.
     * @param args The arguments to be forwarded to the constructor of the object.
     * @return A pointer to the newly created object.
     */
    template <typename T, typename... Args>
    [[nodiscard]] constexpr auto create(Args&&... args) -> T*
    {
        auto* mem = m_pa.allocate_object<T>();
        return std::construct_at(mem, std::forward<Args>(args)...);
    }

    /**
     * Get a reference to the polymorphic allocator used by the memory pool.
     *
     * @return std::pmr::polymorphic_allocator used by the pool
     */
    [[nodiscard]] constexpr auto getAllocator() const noexcept -> auto& { return m_pa; }

    /**
     * Release memory held by the monotonic_buffer_resource
     */
    void reset() noexcept { m_mbr.release(); }

private:
    std::pmr::monotonic_buffer_resource m_mbr;
    std::pmr::polymorphic_allocator<std::byte> m_pa { &m_mbr };
};
} // namespace support

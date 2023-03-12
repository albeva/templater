//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
namespace templater {

class Context final {
public:
    [[nodiscard]] auto allocate(size_t bytes, unsigned alignment) -> void*
    {
        return m_mbr.allocate(bytes, alignment);
    }

    template <typename T, typename... Args>
    [[nodiscard]] auto create(Args&&... args) -> T*
    {
        T* res = static_cast<T*>(allocate(sizeof(T), alignof(T)));
        return std::construct_at(res, std::forward<Args>(args)...);
    }

    template <typename T>
    [[nodiscard]] auto vector() -> std::pmr::vector<T>
    {
        return std::pmr::vector<T>(m_pa);
    }

private:
    std::pmr::monotonic_buffer_resource m_mbr {};
    std::pmr::polymorphic_allocator<std::byte> m_pa { &m_mbr };
};

} // namespace templater

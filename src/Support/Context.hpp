//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
namespace templater {

class Context final {
public:
    [[nodiscard]] void* allocate(size_t bytes, unsigned alignment)
    {
        return m_mbr.allocate(bytes, alignment);
    }

    template <typename T, typename... Args>
    T* create(Args&&... args)
    {
        T* res = static_cast<T*>(allocate(sizeof(T), alignof(T)));
        return std::construct_at(res, std::forward<Args>(args)...);
    }

    template <typename T>
    std::pmr::vector<T> vector()
    {
        return std::pmr::vector<T>(m_ma);
    }

private:
    std::pmr::monotonic_buffer_resource m_mbr {};
    std::pmr::polymorphic_allocator<std::byte> m_ma { &m_mbr };
};

} // namespace templater

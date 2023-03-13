//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
namespace templater {

class Context final {
public:
    template <typename T, typename... Args>
    [[nodiscard]] auto create(Args&&... args) -> T*
    {
        auto* res = m_pa.allocate_object<T>();
        return std::construct_at(res, std::forward<Args>(args)...);
    }

    [[nodiscard]] auto getAllocator() -> auto& { return m_pa; }

private:
    std::pmr::monotonic_buffer_resource m_mbr {};
    std::pmr::polymorphic_allocator<std::byte> m_pa { &m_mbr };
};

} // namespace templater

//
// Created by Albert on 12/03/2023.
//
#pragma once
#include "pch.hpp"
namespace support {

class Context final {
public:
    template <typename T, typename... Args>
    [[nodiscard]] inline auto create(Args&&... args) -> T*
    {
        auto* res = m_pa.allocate_object<T>();
        return std::construct_at(res, std::forward<Args>(args)...);
    }

    [[nodiscard]] inline auto getAllocator() const noexcept -> auto& { return m_pa; }

    [[nodiscard]] inline auto retain(std::string&& str) -> std::string_view
    {
        return *m_uniquedStrings.emplace(std::move(str)).first;
    }

    [[nodiscard]] inline auto retain(std::string_view str) -> std::string_view
    {
        return *m_uniquedStrings.emplace(str).first;
    }

private:
    std::pmr::monotonic_buffer_resource m_mbr {};
    std::pmr::polymorphic_allocator<std::byte> m_pa { &m_mbr };
    pmr::StringSet m_uniquedStrings { m_pa };
};

} // namespace support

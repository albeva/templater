//
// Created by Albert on 18/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
namespace support {
class Source;
}

namespace table::ast {

class Context final {
public:
    NO_COPY_AND_MOVE(Context)
    explicit Context(support::Source* source);
    ~Context();

    /**
     * Allocate and create ast node T. Destructor is never called for Ast nodes,
     * instead whole memory pool is freed at once when Context deallocates.
     */
    template <class T, class... Args>
    [[nodiscard]] inline auto node(Args&&... args) -> T*
    {
        auto obj = m_pa.allocate_object<T>();
        return std::construct_at(obj, std::forward<Args>(args)...);
    }

    template <class T>
    [[nodiscard]] inline auto list() -> List<T>
    {
        return std::pmr::vector<T>(m_pa);
    }

    void setRoot(Content* root) noexcept { m_root = root; }
    [[nodiscard]] inline auto getRoot() const noexcept -> Content* { return m_root; }
    [[nodiscard]] inline auto getSource() const noexcept -> support::Source* { return m_source; }

private:
    std::pmr::monotonic_buffer_resource m_mbr;
    std::pmr::polymorphic_allocator<std::byte> m_pa;
    Content* m_root;
    support::Source* m_source;
};

} // namespace table::ast
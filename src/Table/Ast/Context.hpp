//
// Created by Albert on 18/03/2023.
//
#pragma once
#include "pch.hpp"
#include "Ast.hpp"
#include "Support/MemoryPool.hpp"
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
        return m_pool.create<T>(std::forward<Args>(args)...);
    }

    template <class T>
    [[nodiscard]] inline auto list() -> List<T>
    {
        return std::pmr::vector<T>(m_pool.getAllocator());
    }

    [[nodiscard]] inline auto getSource() const noexcept -> support::Source* { return m_source; }

    void setRoot(Content* root) noexcept { m_root = root; }
    [[nodiscard]] inline auto getRoot() const noexcept -> Content* { return m_root; }

private:
    support::MemoryPool m_pool;
    support::Source* m_source;
    Content* m_root;
};

} // namespace table::ast
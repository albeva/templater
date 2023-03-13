//
// Created by Albert on 06/03/2023.
//
#pragma once
#include "pch.hpp"
#include "SourceLoc.hpp"

namespace templater {

class SourceException final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Source final {
public:
    // Difference from SourceLoc is that it contains
    // human friendly line, col and length info, rather
    // than pointer pair range
    struct Position final {
        unsigned line, col, len;
    };

    explicit Source(const std::filesystem::path& path);
    Source(std::string name, std::string source);

    [[nodiscard]] auto getName() const -> const auto& { return m_name; }

    [[nodiscard]] auto getString(std::size_t line) const -> std::string_view;
    [[nodiscard]] auto getString(SourceLoc loc) const -> std::string_view;

    [[nodiscard]] auto getPosition(SourceLoc loc) const -> Position;
    [[nodiscard]] auto highlight(Position pos) const -> std::string;

    [[nodiscard]] auto data() const -> const char* { return m_source.data(); }
    [[nodiscard]] auto end() const -> const char*
    {
        const auto* end = data();
        std::advance(end, m_source.length());
        return end;
    }

    [[nodiscard]] auto length() const -> std::size_t { return m_source.length(); }

private:
    void verify(const SourceLoc& loc) const;
    [[nodiscard]] auto getLineStart(size_t line) const -> const char*;

    std::string m_name;
    std::string m_source;
};

} // namespace templater

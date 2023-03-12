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
    struct LineAndCol final {
        unsigned line, col;
    };

    explicit Source(const std::filesystem::path& path);
    Source(std::string name, std::string source);

    [[nodiscard]] auto getLineAndCol(SourceLoc loc) const -> LineAndCol;
    [[nodiscard]] auto getString(std::size_t line) const -> std::string_view;
    [[nodiscard]] auto getString(SourceLoc loc) const -> std::string_view;

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

    std::string m_name;
    std::string m_source;
};

} // namespace templater

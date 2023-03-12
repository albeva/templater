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

    explicit Source(const fs::path& path);
    Source(std::string name, std::string source);

    [[nodiscard]] LineAndCol getLineAndCol(SourceLoc loc) const;
    [[nodiscard]] std::string_view getString(std::size_t line) const;
    [[nodiscard]] std::string_view getString(SourceLoc loc) const;

    [[nodiscard]] const char* data() const { return m_source.data(); }
    [[nodiscard]] const char* end() const
    {
        const auto* end = data();
        std::advance(end, m_source.length());
        return end;
    }
    [[nodiscard]] std::size_t length() const { return m_source.length(); }

private:
    void verify(const SourceLoc& loc) const;

    std::string m_name;
    std::string m_source;
};

} // namespace templater

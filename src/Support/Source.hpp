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
    explicit Source(const fs::path& path);
    Source(std::string name, std::string source);
    [[nodiscard]] std::pair<std::size_t, std::size_t> getLineAndCol(SourceLoc loc) const;
    [[nodiscard]] std::string_view getLine(std::size_t line, SourceLoc highlight = {}) const;

    [[nodiscard]] const char* data() const { return m_source.data(); }
    [[nodiscard]] std::size_t length() const { return m_source.length(); }

private:
    Source* m_parent = nullptr;
    SourceLoc m_includeLoc = {};
    std::string m_name;
    std::string m_source;
};

} // namespace templater

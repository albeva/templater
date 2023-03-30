//
// Created by Albert on 06/03/2023.
//
#pragma once
#include "pch.hpp"
#include "SourceLoc.hpp"
#include <string>
namespace support {

class SourceException final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Source final {
public:
    NO_COPY_AND_MOVE(Source)
    explicit Source(const std::filesystem::path& path);
    Source(std::string name, std::string source) noexcept;
    ~Source() = default;

    [[nodiscard]] inline auto getName() const noexcept -> const auto& { return m_name; }

    [[nodiscard]] auto getPosition(SourceLoc loc) const -> SourcePos;
    [[nodiscard]] auto getLine(std::size_t line) const noexcept -> std::optional<std::string_view>;
    [[nodiscard]] auto highlight(SourcePos pos) const -> std::string;

    [[nodiscard]] inline auto buffer() const noexcept -> std::string_view { return m_buffer; }

private:
    using Range = std::pair<std::string::const_iterator, std::string::const_iterator>;
    [[nodiscard]] auto getRange(const SourceLoc& loc) const -> Range;

    std::string m_name;
    std::string m_buffer;
};

} // namespace support

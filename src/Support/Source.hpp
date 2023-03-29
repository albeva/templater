//
// Created by Albert on 06/03/2023.
//
#pragma once
#include "pch.hpp"
#include "SourceLoc.hpp"
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

    [[nodiscard]] auto getLine(std::size_t line) const noexcept -> std::optional<std::string_view>;
    [[nodiscard]] auto getString(SourceLoc loc) const -> std::string_view;

    [[nodiscard]] auto getPosition(SourceLoc loc) const -> SourcePos;
    [[nodiscard]] auto highlight(SourcePos pos) const -> std::string;

    [[nodiscard]] inline auto source() const noexcept -> std::string_view { return m_source; }

    [[nodiscard]] inline auto data() const noexcept -> const char* { return m_source.data(); }
    [[nodiscard]] inline auto end() const noexcept -> const char* { return m_source.data() + m_source.length(); /*  NOLINT */ }
    [[nodiscard]] inline auto length() const noexcept -> std::size_t { return m_source.length(); }

private:
    using Range = std::pair<const char*, const char*>;

    [[nodiscard]] auto normalize(const SourceLoc& loc) const -> Range;

    std::string m_name;
    std::string m_source;
};

} // namespace support

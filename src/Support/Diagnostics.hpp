//
// Created by Albert on 14/03/2023.
//
#pragma once
#include "pch.hpp"
namespace support {
class Source;
struct SourceLoc;

class EmptyException final { };

class Diagnostics final {
public:
    enum class Level {
        Error,
        Notice
    };

    NO_COPY_AND_MOVE(Diagnostics)
    explicit Diagnostics(std::ostream& output) noexcept;
    ~Diagnostics() = default;

    void print(Level level, const Source* source, const SourceLoc& loc, const std::string& message);

    inline void error(const Source* source, const SourceLoc& loc, const std::string& message)
    {
        print(Level::Error, source, loc, message);
    }

    inline void notice(const Source* source, const SourceLoc& loc, const std::string& message)
    {
        print(Level::Notice, source, loc, message);
    }

    [[nodiscard]] inline auto hasErrors() const noexcept { return m_hasErrors; }

private:
    std::ostream& m_output; // NOLINT cppcoreguidelines-avoid-const-or-ref-data-members
    bool m_hasErrors;
};

} // namespace support

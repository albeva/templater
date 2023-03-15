//
// Created by Albert on 14/03/2023.
//
#pragma once
#include "pch.hpp"
namespace templater::support {
class Source;
struct SourceLoc;

class Diagnostics final {
public:
    enum class Level {
        Error,
        Notice
    };

    NO_COPY_AND_MOVE(Diagnostics)
    explicit Diagnostics(std::ostream& output);
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

    [[nodiscard]] auto hasErrors() const { return m_errorCount != 0; }

private:
    std::ostream& m_output; // cppcoreguidelines-avoid-const-or-ref-data-members
    int m_errorCount;
};

} // namespace templater::support

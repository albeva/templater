//
// Created by Albert on 20/03/2023.
//
#pragma once
#include "pch.hpp"
#include "GlobalContext.hpp"
#include "SourceLoc.hpp"
#include "TokenBase.hpp"

namespace support {

/**
 * @concept TokenInformation
 * @tparam Info A type representing token information.
 * @tparam Token A type representing tokens; defaults to the `Token` type nested within `Info`.
 * @tparam Kind A type representing token kinds; defaults to the `Kind` type nested within `Token`.
 *
 * This concept checks whether a given type `Info` satisfies the requirements for token information
 * used by the `LexerBase` class. It requires that `Kind` is a scoped enumeration and defines the
 * following enumerator values: `Invalid`, `String`, `Number`, and `Identifier`. Additionally, it
 * requires a static member function `getKind` in `Info` that takes a `std::string_view` and returns
 * a `Kind` value.
 */
template <typename Token, typename Kind = typename Token::Kind>
concept TokenInformation
    = std::is_scoped_enum_v<Kind>
    && requires {
           Kind::Invalid;
           Kind::String;
           Kind::Number;
           Kind::Identifier;
           {
               Token::getKind(std::string_view {})
           } -> std::same_as<Kind>;
       };

/**
 * @class LexerBase
 * @tparam Info A type satisfying the `TokenInformation` concept.
 *
 * This class provides a basic templated lexer that abstracts common operations for lexing source
 * code. It is intended to be subclassed and extended with language-specific rules.
 */
template <TokenInformation Token>
class LexerBase {
protected:
    NO_COPY_AND_MOVE(LexerBase)

    using Kind = typename Token::Kind;

    /**
     * @brief Checks if the given character is a digit.
     * @param ch The character to check.
     * @return `true` if the character is a digit, `false` otherwise.
     */
    [[nodiscard]] static constexpr auto isDigit(char ch) noexcept -> bool
    {
        return ch >= '0' && ch <= '9';
    }

    /**
     * @brief Checks if the given character is an alphabetic character.
     * @param ch The character to check.
     * @return `true` if the character is an alphabetic character, `false` otherwise.
     */
    [[nodiscard]] static constexpr auto isAlpha(char ch) noexcept -> bool
    {
        return (ch >= 'a' && ch <= 'z')
            || (ch >= 'A' && ch <= 'Z');
    }

    /**
     * @brief Checks if the given character is a valid identifier character.
     * @param ch The character to check.
     * @return `true` if the character is a valid identifier character, `false` otherwise.
     */
    [[nodiscard]] static constexpr auto isIdentChar(char ch) noexcept -> bool
    {
        return isAlpha(ch) || isDigit(ch) || ch == '_';
    }

    /**
     * @brief Checks if the given character represents a line end.
     * @param ch The character to check.
     * @return `true` if the character represents a line end, `false` otherwise.
     */
    [[nodiscard]] static constexpr auto isLineEnd(char ch) noexcept -> bool
    {
        return ch == '\n' || ch == '\r';
    }

    /**
     * @brief Checks if the given character represents horizontal white space.
     * @param ch The character to check.
     * @return `true` if the character represents a space, `false` otherwise.
     */
    [[nodiscard]] static constexpr auto isSpace(char ch) noexcept -> bool
    {
        return ch == '\t' || ch == ' ';
    }

    /**
     * @brief Constructs a `LexerBase` object with the given context and source.
     * @param ctx A pointer to the `support::GlobalContext` object.
     * @param buffer A string_view containing source code to tokenize.
     */
    LexerBase(support::GlobalContext* ctx, std::string_view buffer) noexcept
        : m_ctx(ctx)
        , m_buffer(buffer)
        , m_input(buffer.begin())
    {
    }

    /**
     * @brief Destructor.
     *
     * Virtual destructor ensures proper cleanup of resources when the derived objects are deleted.
     */
    virtual ~LexerBase() = default;

    /**
     * @brief Gets the global context.
     * @return A pointer to the `support::GlobalContext` object.
     */
    [[nodiscard]] inline auto getContext() const noexcept { return m_ctx; }

    /**
     * @brief Constructs a token with the specified kind and advances the input by the given length.
     * @param[out] token The token object to modify.
     * @param kind The token kind.
     * @param len The length to advance the input (default: 1).
     */
    void make(Token& token, Kind kind, std::size_t len = 1) noexcept
    {
        const auto start = m_input;
        std::advance(m_input, len);
        token.set(kind, loc(start));
    }

    /**
     * @brief Constructs an invalid token with the specified error message and advances the input
     *        by one character, if the input is not at the end.
     * @param[out] token The token object to modify.
     * @param message The error message for the invalid token (default: empty string).
     *        If an empty string is passed, the current character is used as the error message.
     */
    void unexpected(Token& token, std::string_view message = {}) noexcept
    {
        const auto start = m_input;
        if (isValid()) {
            std::advance(m_input, 1);
        }
        if (message.empty()) {
            message = lexeme(start, m_input);
        }
        token.set(Kind::Invalid, loc(start), message);
    }

    /**
     * @brief Constructs a string token by lexing a string literal.
     *
     * This function constructs a string token by lexing a string literal from the input stream.
     * It handles escape sequences and constructs the token with the lexed value. If the string
     * literal is not properly terminated or contains an invalid escape sequence, it creates an
     * invalid token with an appropriate error message.
     *
     * @param[out] token The token object to modify.
     */
    void string(Token& token)
    {
        const auto start = m_input++;
        auto begin = m_input;
        std::string literal {};

        while (true) {
            if (!isValid()) {
                return unexpected(token, "open string");
            }
            switch (*m_input) {
            case '\n':
            case '\r':
                return unexpected(token, "open string");
            case '\\': {
                if (begin < m_input) {
                    literal.append(lexeme(begin, m_input));
                }
                if (auto esc = escape()) {
                    literal += esc.value();
                } else {
                    return unexpected(token, esc.error());
                }
                begin = ++m_input;
                continue;
            }
            case '"':
                break;
            default:
                ++m_input;
                continue;
            }
            break;
        }

        auto lit = [&]() {
            if (literal.empty()) {
                return lexeme(begin, m_input);
            }
            if (begin < m_input) {
                literal.append(lexeme(begin, m_input));
            }
            // retain string as it differs from the source input
            return m_ctx->retain(std::move(literal));
        }();

        ++m_input;
        token.set(Kind::String, loc(start), lit);
    }

    /**
     * @brief Processes an escape sequence and returns the corresponding character.
     *
     * This method assumes that the current input character is a backslash ('\\') and processes
     * the character following it as part of an escape sequence. If the escape sequence is valid,
     * the corresponding character is returned. If the escape sequence is invalid, an error message
     * is returned.
     *
     * @return A `std::expected<char, std::string_view>` object containing either the corresponding
     * character of a valid escape sequence or an error message if the escape sequence is invalid.
     */
    [[nodiscard]] auto escape() noexcept -> std::expected<char, std::string_view>
    {
        using p = std::pair<char, char>;
        static constexpr std::array escapes = {
            p { 'a', '\a' }, p { 'b', '\b' }, p { 'f', '\f' }, p { 'n', '\n' },
            p { 'r', '\r' }, p { 't', '\t' }, p { 'v', '\v' }, p { '\\', '\\' },
            p { '\'', '\'' }, p { '"', '"' }
        };

        if (auto it = std::ranges::find(escapes, *++m_input, &p::first); it != escapes.end()) {
            return it->second;
        }
        return std::unexpected("invalid escape sequence");
    }

    /**
     * @brief Lexes a number token from the input.
     * @param[out] token The token object to modify.
     *
     * This method reads a sequence of digits from the input, constructs a number token with the
     * corresponding lexeme, and advances the input past the digits. If the input does not start
     * with a digit, the behavior is undefined.
     */
    void number(Token& token)
    {
        const auto start = m_input++;
        while (isValid() && isDigit(*m_input)) {
            ++m_input;
        }
        token.set(Kind::Number, loc(start), lexeme(start, m_input));
    }

    /**
     * @brief Constructs an identifier token or a keyword token.
     *
     * This function scans the input for a valid identifier and, depending on the lexeme, constructs
     * either an identifier token or a keyword token. The lexeme is checked against the `Info::getKind`
     * function to determine its token kind. If the token kind is `Kind::Identifier`, the lexeme is
     * retained in the global context, and the token is constructed with the retained lexeme.
     *
     * @param[out] token The token object to modify.
     */
    void identifier(Token& token)
    {
        const auto start = m_input++;
        while (isValid() && isIdentChar(*m_input)) {
            ++m_input;
        }

        auto id = lexeme(start, m_input);
        auto kind = Token::getKind(id);
        if (kind == Kind::Identifier) {
            token.set(Kind::Identifier, loc(start), m_ctx->retain(id));
        } else {
            token.set(kind, loc(start));
        }
    }

    /**
     * @fn auto loc(const char* start) noexcept -> support::SourceLoc
     * @brief Computes the location of a token in the source buffer.
     * @param start A pointer to the starting character of the token.
     * @return A `support::SourceLoc` object representing the location of the token.
     *
     * It calculates the starting and ending positions of a token based
     * on the given `start` pointer and the current input position.
     */
    auto loc(std::string_view::iterator start) noexcept -> support::SourceLoc
    {
        return {
            static_cast<unsigned>(std::distance(m_buffer.begin(), start)),
            static_cast<unsigned>(std::distance(m_buffer.begin(), m_input))
        };
    }

    /**
     * @brief Advances the lexer's position in the input buffer by the specified length.
     * @param len The length to advance the lexer's position. By default advance by 1
     *
     * This method updates the lexer's position in the input buffer, effectively skipping over a
     * certain number of characters. It is useful for advancing the lexer's position after parsing
     * a token or for skipping whitespace and comments.
     */
    inline void advance(std::size_t len = 1) noexcept { std::advance(m_input, len); }

    /**
     * @brief Checks that current internal index is valid position in the source buffer
     * @return true if current index is valid, false otherwise
     */
    [[nodiscard]] inline auto isValid() const noexcept { return m_input != m_buffer.end(); }

    /**
     * @brief Gets the size of the input buffer.
     * @return The size of the buffer
     */
    [[nodiscard]] inline auto length() const noexcept { return m_buffer.size(); }

    /**
     * @brief Gets the character at the current index in the input buffer.
     * @return The character at the specified index.
     */
    [[nodiscard]] inline auto current() const noexcept { return *m_input; }

    /**
     * @brief Gets the character at the specified index in the input buffer.
     * @param index The index of the character to retrieve.
     * @return The character at the specified index.
     *
     * This method provides access to individual characters in the input buffer, allowing the lexer
     * to inspect specific characters during parsing.
     */
    [[nodiscard]] inline auto peek() const noexcept { return m_input[1]; }

    /**
     * @brief Gets a substring of the input buffer representing a lexeme.
     * @param start The starting index of the lexeme.
     * @param end The ending index of the lexeme (exclusive).
     * @return A substring representing the lexeme between the start and end indices.
     *
     * This method is useful for extracting lexemes (tokens) from the input buffer during the lexing
     * process. The returned substring can be used to represent the value of a token, such as an
     * identifier or a string literal.
     */
    [[nodiscard]] static inline auto lexeme(std::string_view::iterator start, std::string_view::iterator end) noexcept
    {
        return std::string_view { start, end };
    }

private:
    support::GlobalContext* m_ctx;
    std::string_view m_buffer;
    std::string_view::iterator m_input;
};

} // namespace support

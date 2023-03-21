//
// Created by Albert on 19/03/2023.
//
#pragma once
#include "pch.hpp"
#include "SourceLoc.hpp"
namespace support {

template <typename Info, typename Kind = typename Info::Kind>
concept TokenInformation2
    = std::is_scoped_enum_v<Kind>
    && requires {
           typename Info::Kind;
           {
               Info::describe(Kind {})
           } -> std::same_as<std::string_view>;
           {
               Info::getKind(std::string_view {})
           } -> std::same_as<Kind>;
       };

/**
 * @brief A base class for tokens produced by a lexer.
 *
 * This class is a generic template for defining tokens used in a lexer.
 * It contains methods for setting, getting, and checking properties of the tokens.
 *
 * @tparam K The type of the token's kind. Usually an enum or integral type.
 * @tparam V The type of the token's value. Defaults to std::string_view.
 */
template <TokenInformation2 Info, typename V = std::string_view>
struct TokenBase {
    using Kind = typename Info::Kind;
    using Value = V;

    constexpr TokenBase() noexcept = default;
    constexpr TokenBase(Kind kind, support::SourceLoc loc, Value value = {}) noexcept
        : m_kind(kind)
        , m_loc(loc)
        , m_value(value)
    {
    }

    /**
     * @brief Sets the token properties.
     *
     * @param kind The type of the token.
     * @param loc The source location where the token appears.
     * @param value The value of the token (default: empty).
     */
    constexpr void set(Kind kind, support::SourceLoc loc, Value value = {}) noexcept
    {
        m_kind = kind;
        m_loc = loc;
        m_value = value;
    }

    /**
     * @brief Gets the token kind.
     *
     * @return The kind of the token.
     */
    [[nodiscard]] constexpr auto getKind() const noexcept -> Kind { return m_kind; }

    /**
     * @brief Gets the token location.
     *
     * @return A const reference to the token's source location.
     */
    [[nodiscard]] constexpr auto getLoc() const noexcept -> const support::SourceLoc& { return m_loc; }

    /**
     * @brief Gets the token value.
     *
     * @return A const reference to the token's value.
     */
    [[nodiscard]] constexpr auto getValue() const noexcept -> const Value& { return m_value; }

    /**
     * @brief Checks if the token is of a specific kind.
     *
     * @param kind The kind to compare with.
     * @return True if the token is of the specified kind, false otherwise.
     */
    [[nodiscard]] constexpr auto is(Kind kind) const noexcept -> bool { return m_kind == kind; }

    /**
     * @brief Checks if the token is of one of the specified kinds.
     *
     * @tparam Ty The list of kinds to compare with.
     * @param kind The first kind to compare with.
     * @param ty The other kinds to compare with.
     * @return True if the token is of any of the specified kinds, false otherwise.
     */
    template <typename... Ty>
    [[nodiscard]] constexpr auto is(Kind kind, Ty... ty) const noexcept -> bool { return is(kind) || is(ty...); }

    /**
     * @brief Checks that is not of a specific kind.
     *
     * @param kind The kind to compare with.
     * @return True if the token is not of the specified kind, false otherwise.
     */
    [[nodiscard]] constexpr auto isNot(Kind kind) const noexcept -> bool { return m_kind != kind; }

    /**
     * @brief Checks that token is not of any of the specified kinds.
     *
     * @tparam Ty The list of kinds to compare with.
     * @param kind The first kind to compare with.
     * @param ty The other kinds to compare with.
     * @return True if the token is not of any of the specified kinds, false otherwise.
     */
    template <typename... Ty>
    [[nodiscard]] constexpr auto isNot(Kind kind, Ty... ty) const noexcept -> bool { return isNot(kind) && isNot(ty...); }

    /**
     * @brief Get token description
     * @return string_view describing the token
     */
    [[nodiscard]] auto description() const noexcept -> std::string_view { return Info::describe(m_kind); }

    /**
     * Get description for given Kind
     * @param kind
     * @return string_view
     */
    [[nodiscard]] static auto describe(Kind kind) noexcept -> std::string_view { return Info::describe(kind); }

    /**
     * Get matching Kind for potential keyword
     */
    [[nodiscard]] static auto getKind(std::string_view id) noexcept -> Kind { return Info::getKind(id); }

private:
    Kind m_kind {};
    support::SourceLoc m_loc {};
    Value m_value {};
};

} // namespace support

/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

#include <type_traits>

namespace smile::primitive
{
    namespace detail
    {
        template < typename Type, Index >
        requires std::is_nothrow_default_constructible_v< Type >
        class CompressedPairElement
        {
          public:
            constexpr CompressedPairElement() noexcept : m_Value{}
            {
            }

            template < typename Arg >
            constexpr CompressedPairElement( Arg &&arg ) noexcept
            requires std::is_nothrow_constructible_v< Type, Arg > && !std::is_same_v< std::remove_cvref_t< Arg >, Type >
                : m_Value{ std::forward< Arg >( arg ) }
            {
            }

            template < typename... Args, Index... Idx >
            constexpr CompressedPairElement( std::tuple< Args... > args, std::index_sequence< Idx... > ) noexcept
            requires std::is_nothrow_constructible_v< Type, Args... >
                : m_Value{ std::forward< Args >( std::get< Idx >( args ) )... }
            {
            }

            [[nodiscard]] constexpr CompressedPairElement &Get() noexcept
            {
                return m_Value;
            }

            [[nodiscard]] constexpr const CompressedPairElement &Get() const noexcept
            {
                return m_Value;
            }

          private:
            Type m_Value;
        };
    }

    template < typename First, typename Second >
    requires std::is_default_constructible_v< First > && std::is_default_constructible_v< Second >
    class CompressedPair final : private detail::CompressedPairElement< First, 0u >,
                                 private detail::CompressedPairElement< Second, 1u >
    {
        using FirstBase = detail::CompressedPairElement< First, 0u >;
        using SecondBase = detail::CompressedPairElement< Second, 1u >;

      public:
        using FirstType = First;
        using SecondType = Second;

        constexpr CompressedPair() noexcept
        requires std::is_nothrow_default_constructible_v< FirstBase > &&
                     std::is_nothrow_default_constructible_v< SecondBase >
            : FirstBase{}, SecondBase{}
        {
        }

        constexpr CompressedPair( const CompressedPair & ) noexcept
        requires std::is_nothrow_copy_constructible_v< FirstBase > && std::is_nothrow_copy_constructible_v< SecondBase >
        = default;

        constexpr CompressedPair( CompressedPair && ) noexcept
        requires std::is_nothrow_move_constructible_v< FirstBase > && std::is_nothrow_move_constructible_v< SecondBase >
        = default;

        template < typename Arg, typename Other >
        constexpr CompressedPair( Arg &&arg, Other &&other ) noexcept
        requires std::is_nothrow_constructible_v< FirstBase, Arg > &&
                     std::is_nothrow_constructible_v< SecondBase, Other >
            : FirstBase{ std::forward< Arg >( arg ) }, SecondBase{ std::forward< Other >( other ) }
        {
        }

        template < typename... Args, typename... Other >
        constexpr CompressedPair( std::piecewise_construct_t,
            std::tuple< Args... > args,
            std::tuple< Other... > other ) noexcept
        requires std::is_nothrow_constructible_v< FirstBase, Args... > &&
                     std::is_nothrow_constructible_v< SecondBase, Other... >
            : FirstBase{ std::move( args ), std::index_sequence_for< Args... >{} },
              SecondBase{ std::move( other, std::index_sequence_for< Other... >{} }
        {
        }

        constexpr CompressedPair &operator=( const CompressedPair &other ) noexcept
        requires std::is_nothrow_copy_assignable_v< FirstBase > && std::is_nothrow_copy_assignable_v< SecondBase >
        = default;

        constexpr CompressedPair &operator=( CompressedPair &&other ) noexcept
        requires std::is_nothrow_move_assignable_v< FirstBase > && std::is_nothrow_move_assignable_v< SecondBase >
        = default;

        [[nodiscard]] constexpr FirstType &First() noexcept
        {
            return static_cast< FirstBase & >( *this ).Get();
        }

        [[nodiscard]] constexpr const FirstType &First() const noexcept
        {
            return static_cast< const FirstBase & >( *this ).Get();
        }

        [[nodiscard]] constexpr SecondType &Second() noexcept
        {
            return static_cast< SecondBase & >( *this ).Get();
        }

        [[nodiscard]] constexpr const SecondType &Second() const noexcept
        {
            return static_cast< const SecondBase & >( *this ).Get();
        }

        template < Index Idx >
        constexpr decltype( auto ) Get() noexcept
        {
            if constexpr ( Idx == 0u )
            {
                return First();
            }
            else
            {
                static_assert( Idx == 1u, "Index must be 0 or 1" );
                return Second();
            }
        }

        template < Index Idx >
        constexpr decltype( auto ) Get() const noexcept
        {
            if constexpr ( Idx == 0u )
            {
                return First();
            }
            else
            {
                static_assert( Idx == 1u, "Index must be 0 or 1" );
                return Second();
            }
        }
    };
}
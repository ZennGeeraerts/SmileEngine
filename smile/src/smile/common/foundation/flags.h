/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "compiled.h"
#include "assert.h"

#include <type_traits>

namespace smile::foundation
{
    template < typename EnumType >
    class Flags final
    {
        static_assert( std::is_enum_v< EnumType > );
        using UnderlyingType = typename std::underlying_type_t< EnumType >;

      public:
        constexpr Flags() = default;

        constexpr Flags( std::initializer_list< EnumType > values )
        {
            Set( values );
        }

        constexpr inline void Set( EnumType value )
        {
            SM_ASSERT_MSG(
                static_cast< UnderlyingType >( value ) < ( 8 * sizeof( UnderlyingType ) ), "Value out of range" );

            m_Flags |= ( 1 << static_cast< UnderlyingType >( value ) );
        }

        constexpr inline void Set( std::initializer_list< EnumType > values )
        {
            for ( auto value : values )
                Set( value );
        }

        constexpr inline bool Has( EnumType value ) const
        {
            SM_ASSERT_MSG(
                static_cast< UnderlyingType >( value ) < ( 8 * sizeof( UnderlyingType ) ), "Value out of range" );

            return ( m_Flags & ( 1 << static_cast< UnderlyingType >( value ) ) ) != 0;
        }

        constexpr inline bool HasAll( std::initializer_list< EnumType > values ) const
        {
            for ( auto value : values )
            {
                SM_ASSERT_MSG(
                    static_cast< UnderlyingType >( value ) < ( 8 * sizeof( UnderlyingType ) ), "Value out of range" );

                if ( ( m_Flags & ( 1 << static_cast< UnderlyingType >( value ) ) ) == 0 )
                    return false;
            }

            return true;
        }

        constexpr inline void Clear( EnumType value )
        {
            SM_ASSERT_MSG(
                static_cast< UnderlyingType >( value ) < ( 8 * sizeof( UnderlyingType ) ), "Value out of range" );

            m_Flags &= ~( 1 << static_cast< UnderlyingType >( value ) );
        }

        constexpr inline void Clear( std::initializer_list< EnumType > values )
        {
            for ( auto value : values )
                Clear( value );
        }

        constexpr inline void Toggle( EnumType value )
        {
            SM_ASSERT_MSG(
                static_cast< UnderlyingType >( value ) < ( 8 * sizeof( UnderlyingType ) ), "Value out of range" );

            m_Flags ^= ( 1 << static_cast< UnderlyingType >( value ) );
        }

        constexpr inline void Toggle( std::initializer_list< EnumType > values )
        {
            for ( auto value : values )
                Toggle( value );
        }

        constexpr inline UnderlyingType GetFlags() const
        {
            return m_Flags;
        }

        constexpr inline void SetFlags( UnderlyingType flags )
        {
            m_Flags = flags;
        }

        constexpr inline bool operator==( const Flags &other ) const
        {
            return m_Flags == other.m_Flags;
        }

      private:
        UnderlyingType m_Flags{ 0 };
    };

    template < typename EnumType >
    inline constexpr Flags< EnumType > operator|( Flags< EnumType > flags, EnumType value )
    {
        auto result = flags;
        result.Set( value );
        return result;
    }

    template < typename EnumType >
    inline constexpr Flags< EnumType > operator&( Flags< EnumType > lhs, Flags< EnumType > rhs )
    {
        Flags< EnumType > result;
        result.SetFlags( lhs.GetFlags() & rhs.GetFlags() );
        return result;
    }
}
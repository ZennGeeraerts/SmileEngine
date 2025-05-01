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

      public:
        constexpr Flags() = default;

        constexpr Flags( std::initializer_list< EnumType > values )
        {
            Set( values );
        }

        constexpr inline void Set( EnumType value )
        {
            SM_ASSERT( static_cast< Uint32 >( value ) < ( 8 * sizeof( Uint32 ) ), "Value out of range" );
            m_Flags |= ( 1 << static_cast< Uint32 >( value ) );
        }

        constexpr inline void Set( std::initializer_list< EnumType > values )
        {
            for ( auto value : values )
                Set( value );
        }

        constexpr inline bool Has( EnumType value ) const
        {
            SM_ASSERT( static_cast< Uint32 >( value ) < ( 8 * sizeof( Uint32 ) ), "Value out of range" );
            return ( m_Flags & ( 1 << static_cast< Uint32 >( value ) ) ) != 0;
        }

        constexpr inline bool HasAll( std::initializer_list< EnumType > values ) const
        {
            for ( auto value : values )
            {
                SM_ASSERT( static_cast< Uint32 >( value ) < ( 8 * sizeof( Uint32 ) ), "Value out of range" );

                if ( ( m_Flags & ( 1 << static_cast< Uint32 >( value ) ) ) == 0 )
                    return false;
            }

            return true;
        }

        constexpr inline void Clear( EnumType value )
        {
            SM_ASSERT( static_cast< Uint32 >( value ) < ( 8 * sizeof( Uint32 ) ), "Value out of range" );
            m_Flags &= ~( 1 << static_cast< Uint32 >( value ) );
        }

        constexpr inline void Clear( std::initializer_list< EnumType > values )
        {
            for ( auto value : values )
                Clear( value );
        }

        constexpr inline Uint32 GetFlags() const
        {
            return m_Flags;
        }

        constexpr inline void SetFlags( Uint32 flags )
        {
            m_Flags = flags;
        }

        constexpr inline bool operator==( const Flags &other ) const
        {
            return m_Flags == other.m_Flags;
        }

      private:
        Uint32 m_Flags{ 0 };
    };
}
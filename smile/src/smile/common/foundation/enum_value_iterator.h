/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        enum_value_iterator.h
 * @author      Zenn Geeraerts
 * @created     28 December 2025
 * @brief       Iterator for values of an enum
 */

#include "compiled.h"
#include "enum_concepts.h"

namespace smile::foundation
{
    template < EnumWithCount EnumType >
    class EnumValueIterator final
    {
        static_assert( std::is_enum_v< EnumType > );

      public:
        constexpr EnumValueIterator( const EnumType value ) : m_Value{ value }
        {
        }

        constexpr EnumType operator*() const
        {
            return m_Value;
        }

        constexpr EnumValueIterator &operator++()
        {
            Value = static_cast< EnumType >( static_cast< int >( Value ) + 1 );

            return *this;
        }

        constexpr bool operator==( const EnumValueIterator &other ) const
        {
            return m_Value == other.m_Value;
        }

        constexpr bool operator!=( const EnumValueIterator &other ) const
        {
            return m_Value != other.m_Value;
        }

      private:
        EnumType m_Value;
    }

    template < EnumWithCount EnumType >
    class EnumValues final
    {
        static_assert( std::is_enum_v< EnumType > );

      public:
        EnumValues() = default;
    };

    template < EnumWithCount EnumType >
    constexpr EnumValueIterator< EnumType > begin( EnumValues< EnumType > )
    {
        return { static_cast< EnumType >( 0 ) };
    }

    template < EnumWithCount EnumType >
    constexpr EnumValueIterator< EnumType > end( EnumValues< EnumType > )
    {
        return { EnumType::Count };
    }
}
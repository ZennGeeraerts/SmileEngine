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
 * @file        enum_array.h
 * @author      Zenn Geeraerts
 * @created     17 December 2025
 * @brief       Array to map enum values to items
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/enum_concepts.h"
#include "smile/common/memory/memory.h"
#include "array_iterator.h"

namespace smile::primitive
{
    template < foundation::EnumWithCount EnumType, typename ItemType >
    class EnumArray final
    {
        static_assert( std::is_enum_v< EnumType > );

      public:
        using Item = ItemType;
        using Iterator = ArrayIterator< EnumArray, false >;
        using ConstIterator = ArrayIterator< EnumArray, true >;

        constexpr EnumArray() = default;

        constexpr EnumArray( std::initializer_list< Item > items )
        {
            SM_ASSERT( items.size() == s_ItemCount );

            Index index{ 0 };
            for ( auto it{ items.begin() }; it != items.end(); ++it )
            {
                m_Items[index] = *it;
                ++index;
            }
        }

        template < int Size >
        constexpr EnumArray( const Item ( &items )[Size] ) noexcept
        {
            static_assert( Size == s_ItemCount );

            for ( auto i{ 0 }; i < Size; ++i )
            {
                m_Items[i] = items[i];
            }
        }

        template < typename OtherItem, int Size >
        constexpr EnumArray( const OtherItem ( &items )[Size] ) noexcept
        {
            static_assert( Size == s_ItemCount );

            for ( auto i{ 0 }; i < Size; ++i )
            {
                m_Items[i] = items[i];
            }
        }

        ~EnumArray() = default;

        constexpr Item &operator[]( const EnumType index ) noexcept
        {
            return m_Items[static_cast< int >( index )];
        }

        constexpr const Item &operator[]( const EnumType index ) const noexcept
        {
            return m_Items[static_cast< int >( index )];
        }

        constexpr Item &operator[]( const int index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_Items[index];
        }

        constexpr const Item &operator[]( const int index ) const
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_Items[index];
        }

        EnumArray &operator=( const EnumArray &other ) noexcept
        {
            Set( other );

            return *this;
        }

        static constexpr bool IsEmpty() noexcept
        {
            return s_ItemCount == 0;
        }

        constexpr Item &GetItemAtIndex( const EnumType index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            return m_Items[index];
        }

        constexpr const Item &GetItemAtIndex( const EnumType index ) const noexcept
        {
            return m_Items[static_cast< int >( index )];
        }

        constexpr Item *GetData() noexcept
        {
            return m_Items;
        }

        constexpr const Item *GetData() const noexcept
        {
            return m_Items;
        }

        static constexpr Count GetItemCount() noexcept
        {
            return s_ItemCount;
        }

        [[nodiscard]] constexpr bool IsValidIndex( const int index ) const noexcept
        {
            return index >= 0 && index < s_ItemCount;
        }

        [[nodiscard]] constexpr bool IsValidIndex( const EnumType index ) const noexcept
        {
            return static_cast< Index >( index ) < s_ItemCount;
        }

        constexpr Iterator begin() noexcept
        {
            return { this, 0 };
        }

        constexpr ConstIterator begin() const noexcept
        {
            return { this, 0 };
        }

        constexpr Iterator end() noexcept
        {
            return { this, s_ItemCount };
        }

        constexpr ConstIterator end() const noexcept
        {
            return { this, s_ItemCount };
        }

        constexpr void SetItemAtIndex( const Item &item, const EnumType index )
        {
            SM_ASSERT( IsValidIndex( index ) );

            m_Items[index] = item;
        }

        void Set( const EnumArray< EnumType, Item > &other ) noexcept
        {
            if constexpr ( memory::IsRawType< Item >() )
            {
                std::memcpy( m_Items, other.m_Items, s_ItemCount * sizeof( Item ) );
            }
            else
            {
                memory::DestructArrayItems( m_Items, s_ItemCount );
                memory::ConstructCopiedArrayItems( m_Items, s_ItemCount, other.m_Items );
            }
        }

      private:
        Item m_Items[static_cast< int >( EnumType::Count )];
        constexpr static Count s_ItemCount{ static_cast< Count >( EnumType::Count ) };
    };
}
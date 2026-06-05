/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "constant_type.h"

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/array_utils.h"

namespace smile::graphic
{
    struct ConstantBufferItem final
    {
        Count GetStride() const noexcept
        {
            return Size * ItemCount;
        }

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash = std::hash< primitive::String >{}( Name );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( static_cast< Uint32 >( Type ) ) );
            hash = foundation::HashCombine( hash, std::hash< Count >{}( Size ) );
            hash = foundation::HashCombine( hash, std::hash< Count >{}( Offset ) );
            hash = foundation::HashCombine( hash, std::hash< Count >{}( ItemCount ) );
            return hash;
        }

        bool operator==( const ConstantBufferItem &other ) const noexcept
        {
            return Name == other.Name && Type == other.Type && Size == other.Size && Offset == other.Offset &&
                   ItemCount == other.ItemCount;
        }

        bool operator!=( const ConstantBufferItem &other ) const noexcept
        {
            return !( *this == other );
        }

        primitive::String Name;
        ConstantType Type;
        Count Size;
        Count Offset;
        Count ItemCount; // Used for arrays
    };

    class ConstantBufferDescriptor final
    {
      public:
        ConstantBufferDescriptor() noexcept : m_Items{}, m_Size{ 0 }
        {
        }

        ConstantBufferDescriptor( std::initializer_list< std::tuple< primitive::String, ConstantType, Count > > list );

        void Add( primitive::String name, ConstantType type, const Count itemCount = 1 );
        void Add( const ConstantBufferItem &item );

        Count GetSize() const noexcept
        {
            return m_Size;
        }

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash = std::hash< primitive::Vector< ConstantBufferItem > >{}( m_Items );
            hash = foundation::HashCombine( hash, std::hash< Count >{}( m_Size ) );
            return hash;
        }

        auto begin() noexcept
        {
            return m_Items.begin();
        }

        auto end() noexcept
        {
            return m_Items.end();
        }

        auto begin() const noexcept
        {
            return m_Items.begin();
        }

        auto end() const noexcept
        {
            return m_Items.end();
        }

        bool operator==( const ConstantBufferDescriptor &other ) const noexcept
        {
            return primitive::array::IsEqual( m_Items, other.m_Items ) && m_Size == other.m_Size;
        }

        bool operator!=( const ConstantBufferDescriptor &other ) const noexcept
        {
            return !( *this == other );
        }

      private:
        void CalculateOffsetAndSize() noexcept;

      private:
        primitive::Vector< ConstantBufferItem > m_Items;
        Count m_Size;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::ConstantBufferItem >
    {
        smile::foundation::HashCode operator()( const smile::graphic::ConstantBufferItem &item ) const
        {
            return item.GetHashCode();
        }
    };

    template <>
    struct hash< smile::graphic::ConstantBufferDescriptor >
    {
        smile::foundation::HashCode operator()( const smile::graphic::ConstantBufferDescriptor &descriptor ) const
        {
            return descriptor.GetHashCode();
        }
    };
}
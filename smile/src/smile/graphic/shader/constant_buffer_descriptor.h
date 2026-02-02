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
        Count GetStride() const
        {
            return Size * ItemCount;
        }

        inline bool operator==( const ConstantBufferItem &other ) const
        {
            return Name == other.Name && Type == other.Type && Size == other.Size && Offset == other.Offset &&
                   ItemCount == other.ItemCount;
        }

        inline bool operator!=( const ConstantBufferItem &other ) const
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
        ConstantBufferDescriptor() : m_Items{}, m_Size{ 0 }
        {
        }

        ConstantBufferDescriptor( std::initializer_list< std::tuple< primitive::String, ConstantType, Count > > list );

        void Add( primitive::String name, ConstantType type, const Count itemCount = 1 );
        void Add( const ConstantBufferItem &item );

        Count GetSize() const
        {
            return m_Size;
        }

        auto begin()
        {
            return m_Items.begin();
        }

        auto end()
        {
            return m_Items.end();
        }

        auto begin() const
        {
            return m_Items.begin();
        }

        auto end() const
        {
            return m_Items.end();
        }

        inline bool operator==( const ConstantBufferDescriptor &other ) const
        {
            return primitive::array::IsEqual( m_Items, other.m_Items ) && m_Size == other.m_Size;
        }

        inline bool operator!=( const ConstantBufferDescriptor &other ) const
        {
            return !( *this == other );
        }

      private:
        void CalculateOffsetAndSize();

      private:
        primitive::Vector< ConstantBufferItem > m_Items;
        Count m_Size;
    };
}
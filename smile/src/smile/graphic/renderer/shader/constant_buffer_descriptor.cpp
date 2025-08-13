/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "constant_buffer_descriptor.h"

namespace smile::graphic
{
    ConstantBufferDescriptor::ConstantBufferDescriptor(
        std::initializer_list< std::tuple< primitive::String, ConstantType, Count > > list )
        : m_Items{}, m_Size{}
    {
        for ( auto &item : list )
        {
            Add( std::get< 0 >( item ), std::get< 1 >( item ), std::get< 2 >( item ) );
        }
    }

    void ConstantBufferDescriptor::Add( primitive::String name, ConstantType type, const Count itemCount )
    {
        const Count size = graphic::GetConstantTypeInfo( type ).Size;
        m_Items.EmplaceBack( std::move( name ), type, size, itemCount );
        CalculateOffsetAndSize();
    }

    void ConstantBufferDescriptor::Add( const ConstantBufferItem &item )
    {
        m_Items.PushBack( item );
        CalculateOffsetAndSize();
    }

    void ConstantBufferDescriptor::CalculateOffsetAndSize()
    {
        Count offset{ 0 };
        m_Size = 0;

        for ( auto &item : m_Items )
        {
            item.Offset = offset;
            offset += item.Size * item.ItemCount;
            m_Size += item.Size * item.ItemCount;
        }
    }
}
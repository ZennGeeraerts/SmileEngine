/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "constant_buffer_descriptor.h"

namespace smile::graphic
{
    ConstantBufferDescriptor::ConstantBufferDescriptor(
        std::initializer_list< std::tuple< primitive::String, ConstantType, Count, Count > > list )
        : m_Items{}, m_Size{}
    {
        for ( auto &item : list )
        {
            Add( std::get< 0 >( item ), std::get< 1 >( item ), std::get< 2 >( item ), std::get< 3 >( item ) );
        }
    }

    void
    ConstantBufferDescriptor::Add( primitive::String name, ConstantType type, const Count size, const Count itemCount )
    {
        m_Items.EmplaceBack( std::move( name ), type, size, itemCount );
        m_Size += graphic::GetSize( type );
    }
}
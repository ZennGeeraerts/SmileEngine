/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "resource_type.h"
#include "shader_stage.h"

namespace smile::graphic::rhi
{
    struct BindingLayoutElement final
    {
        BindingLayoutElement() = default;

        BindingLayoutElement( Uint32 slot, ResourceType type ) : Slot{ slot }, Type{ type }, Size{ 0 }
        {
        }

        BindingLayoutElement( Uint32 slot, ResourceType type, Uint16 size ) : Slot{ slot }, Type{ type }, Size{ size }
        {
        }

        inline bool operator==( const BindingLayoutElement &other ) const
        {
            return Slot == other.Slot && Type == other.Type && Size == other.Size;
        }

        inline bool operator!=( const BindingLayoutElement &other ) const
        {
            return !( *this == other );
        }

        Uint32 Slot;
        ResourceType Type;
        Uint16 Size;
    };

    class BindingLayout final
    {
      public:
        BindingLayout() = default;

        BindingLayout( foundation::Flags< ShaderStage > visibility ) : m_Visibility{ visibility }
        {
        }

        BindingLayout( foundation::Flags< ShaderStage > visibility,
            std::initializer_list< BindingLayoutElement > elements )
            : m_Visibility{ visibility }, m_Elements{ elements }
        {
        }

        inline const primitive::Vector< BindingLayoutElement > &GetElements() const
        {
            return m_Elements;
        }

        inline foundation::Flags< ShaderStage > GetVisibility() const
        {
            return m_Visibility;
        }

        primitive::Vector< BindingLayoutElement >::Iterator begin()
        {
            return m_Elements.begin();
        }

        primitive::Vector< BindingLayoutElement >::Iterator end()
        {
            return m_Elements.end();
        }

        primitive::Vector< BindingLayoutElement >::ConstIterator begin() const
        {
            return m_Elements.begin();
        }

        primitive::Vector< BindingLayoutElement >::ConstIterator end() const
        {
            return m_Elements.end();
        }

        void AddElement( const BindingLayoutElement &element )
        {
            m_Elements.PushBack( element );
        }

        void AddElement( BindingLayoutElement &&element )
        {
            m_Elements.EmplaceBack( std::move( element ) );
        }

        bool HasElement( const BindingLayoutElement &element ) const
        {
            return primitive::array::HasItem( m_Elements, element );
        }

        void Clear()
        {
            m_Elements.Clear();
        }

      private:
        primitive::Vector< BindingLayoutElement > m_Elements;
        foundation::Flags< ShaderStage > m_Visibility;
    };
}
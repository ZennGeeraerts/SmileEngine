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
        BindingLayoutElement( Uint32 slot, ResourceType type ) : Slot{ slot }, Type{ type }, Size{ 0 }
        {
        }

        BindingLayoutElement( Uint32 slot, ResourceType type, Uint16 size ) : Slot{ slot }, Type{ type }, Size{ size }
        {
        }

        inline bool operator==(const BindingLayoutElement& other) const
        {
            return Slot == other.Slot && Type == other.Type && Size == other.Size;
        }

        inline bool operator!=(const BindingLayoutElement& other) const
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
            const std::initializer_list< BindingLayoutElement > &elements )
            : m_Visibility{ visibility }, m_Elements{ elements }
        {
        }

        inline const std::vector< BindingLayoutElement > &GetElements() const
        {
            return m_Elements;
        }

        inline foundation::Flags< ShaderStage > GetVisibility() const
        {
            return m_Visibility;
        }

        std::vector< BindingLayoutElement >::iterator begin()
        {
            return m_Elements.begin();
        }

        std::vector< BindingLayoutElement >::iterator end()
        {
            return m_Elements.end();
        }

        std::vector< BindingLayoutElement >::const_iterator begin() const
        {
            return m_Elements.cbegin();
        }

        std::vector< BindingLayoutElement >::const_iterator end() const
        {
            return m_Elements.cend();
        }

        void AddElement( const BindingLayoutElement &element )
        {
            m_Elements.push_back( element );
        }

        void AddElement( BindingLayoutElement &&element )
        {
            m_Elements.emplace_back( std::move( element ) );
        }

        void Clear()
        {
            m_Elements.clear();
        }

      private:
        std::vector< BindingLayoutElement > m_Elements;
        foundation::Flags< ShaderStage > m_Visibility;
    };
}
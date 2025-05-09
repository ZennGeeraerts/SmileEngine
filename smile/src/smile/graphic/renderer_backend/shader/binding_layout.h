/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "resource_type.h"
#include "shader_stage.h"

namespace smile::graphic
{
    struct BindingLayoutElement final
    {
        BindingLayoutElement( Uint32 slot, ResourceType type, Uint16 size ) : Slot{ slot }, Type{ type }, Size{ size }
        {
        }

        Uint32 Slot;
        ResourceType Type;
        Uint16 Size;
    };

    class BindingLayout final
    {
      public:
        BindingLayout( ShaderStage visibility ) : m_Visiblity{ visibility }
        {
        }

        BindingLayout( ShaderStage visibility, const std::initializer_list< BindingLayoutElement > &elements )
            : m_Visibility{ visibility }, m_Elements{ elements }
        {
        }

        inline const std::vector< BindingLayoutElement > &GetElements() const
        {
            return m_Elements;
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

      private:
        std::vector< BindingLayoutElement > m_Elements;
        ShaderStage m_Visibility;
    };
}
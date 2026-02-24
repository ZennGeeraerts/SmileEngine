/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        program.h
 * @author      Zenn Geeraerts
 * @created     2 February 2026
 * @brief       Unified shader program interface
 */
#pragma once

#include "vertex_shader.h"
#include "pixel_shader.h"

namespace smile::graphic
{
    class Program final : memory::Counted
    {
      public:
        using Ref = memory::Ref< Program >;
        using ConstRef = memory::Ref< const Program >;

        struct Resource final
        {
            Resource( const NamedBindingLayoutElement &namedElement, foundation::Flags< rhi::ShaderStage > visibility )
                : NamedElement{ namedElement }, Visibility{ visibility }
            {
            }

            Resource( const rhi::BindingLayoutElement &element,
                const primitive::String &name,
                foundation::Flags< rhi::ShaderStage > visibility )
                : NamedElement{ element, name }, Visibility{ visibility }
            {
            }

            NamedBindingLayoutElement NamedElement;
            foundation::Flags< rhi::ShaderStage > Visibility;
        };

        VertexShader::ConstRef GetVertexShader() const
        {
            return m_VertexShader;
        }

        PixelShader::ConstRef GetPixelShader() const
        {
            return m_PixelShader;
        }

        const primitive::Vector< Resource > &GetResources() const
        {
            return m_ResourceBindings;
        }

        const ConstantBufferDescriptor &GetConstantBufferDescriptor( const primitive::StringView descName ) const
        {
            return m_ConstantBufferDescs[descName];
        }

        static Program::Ref Create( VertexShader::ConstRef vertexShader, PixelShader::ConstRef pixelShader );

      private:
        Program( VertexShader::ConstRef vertexShader,
            PixelShader::ConstRef pixelShader,
            const primitive::Vector< Program::Resource > &resources,
            const primitive::HashMap< primitive::String, ConstantBufferDescriptor > &cbDescs )
            : m_VertexShader{ vertexShader },
              m_PixelShader{ pixelShader },
              m_ResourceBindings{ resources },
              m_ConstantBufferDescs{ cbDescs }
        {
        }

        VertexShader::ConstRef m_VertexShader;
        PixelShader::ConstRef m_PixelShader;

        primitive::Vector< Resource > m_ResourceBindings;
        primitive::HashMap< primitive::String, ConstantBufferDescriptor > m_ConstantBufferDescs;
    };
}
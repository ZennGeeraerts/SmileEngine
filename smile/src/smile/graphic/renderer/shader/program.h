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

        class BindingLayout final : public rhi::BindingLayout
        {
          public:
            BindingLayout() : rhi::BindingLayout{ { rhi::ShaderStage::Vertex, rhi::ShaderStage::Pixel } }
            {
            }

            bool TryAddElement( const rhi::BindingLayoutElement &element )
            {
                if ( !HasElement( element ) )
                {
                    AddElement( element );
                    return true;
                }

                return false;
            }
        };

        struct ReflectionData final
        {
            rhi::BufferLayout VertexLayout;

            BindingLayout Bindings;
            primitive::HashMap< primitive::String, rhi::BindingLayoutElement > NameToBindingMap;
            primitive::HashMap< primitive::String, ConstantBufferDescriptor > ConstantBufferDescs;
        };

        static Program::Ref Create( VertexShader::ConstRef vertexShader, PixelShader::ConstRef pixelShader );

      private:
        Program( VertexShader::ConstRef vertexShader,
            PixelShader::ConstRef pixelShader,
            const ReflectionData &reflectionData )
            : m_VertexShader{ vertexShader }, m_PixelShader{ pixelShader }, m_ReflectionData{ reflectionData }
        {
        }

        VertexShader::ConstRef m_VertexShader;
        PixelShader::ConstRef m_PixelShader;
        ReflectionData m_ReflectionData;
    };
}
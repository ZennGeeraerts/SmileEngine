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

#include "smile/graphic/shader/shader_asset.h"

namespace smile::graphic
{
    class Program final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Program >;
        using ConstRef = memory::Ref< const Program >;

        struct Resource final
        {
            Resource() = default;

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

        Program( ShaderAsset::ConstRef vertexShader,
            ShaderAsset::ConstRef pixelShader,
            const primitive::Vector< Program::Resource > &resources,
            const primitive::HashMap< primitive::String, ConstantBufferDescriptor > &cbDescs,
            const rhi::BufferLayout &vertexLayout )
            : m_VertexShader{ vertexShader },
              m_PixelShader{ pixelShader },
              m_ResourceBindings{ resources },
              m_ConstantBufferDescs{ cbDescs },
              m_VertexLayout{ vertexLayout }
        {
        }

        ShaderAsset::ConstRef GetVertexShader() const
        {
            return m_VertexShader;
        }

        ShaderAsset::ConstRef GetPixelShader() const
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

        const rhi::BufferLayout &GetVertexLayout() const
        {
            return m_VertexLayout;
        }

        static Program::Ref Create( ShaderAsset::ConstRef vertexShader, ShaderAsset::ConstRef pixelShader );

      private:
        ShaderAsset::ConstRef m_VertexShader;
        ShaderAsset::ConstRef m_PixelShader;

        primitive::Vector< Resource > m_ResourceBindings;
        primitive::HashMap< primitive::String, ConstantBufferDescriptor > m_ConstantBufferDescs;
        rhi::BufferLayout m_VertexLayout;
    };
}
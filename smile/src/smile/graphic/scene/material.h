/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/shader/shader_asset.h"
#include "smile/graphic/renderer/resource/graphics_pipeline.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/hash_map.h"

namespace smile::graphic
{
    class Material final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Material >;
        using ConstRef = memory::Ref< const Material >;

        Material( const ShaderAsset::ConstRef &pVertexShader, const ShaderAsset::ConstRef &pPixelShader ) noexcept;
        ~Material() noexcept;

        void SetShaders( const ShaderAsset::ConstRef &pVertexShader, const ShaderAsset::ConstRef &pPixelShader );

        inline const rhi::BufferLayout &GetBufferLayout() const
        {
            return m_pVertexShader->GetReflectionData().InputSignature;
        }

        inline VertexShader::ConstRef GetVertexShader() const
        {
            return m_pVertexShader->GetVertexShader();
        }

        inline PixelShader::ConstRef GetPixelShader() const
        {
            return m_pPixelShader->GetPixelShader();
        }

        inline GraphicsPipeline::Ref GetGraphicsPipeline() const
        {
            return m_pGraphicsPipeline;
        }

        inline const primitive::HashMap< primitive::String, rhi::BindingLayoutElement > &GetBindings() const
        {
            return m_Bindings;
        }

        inline const ConstantBufferDescriptor &GetConstantBufferDesc( const primitive::StringView name ) const
        {
            return m_ConstantBufferDescs.GetItemAtKey( name );
        }

        inline const primitive::HashMap< primitive::String, ConstantBufferDescriptor > &GetConstantBufferDescs() const
        {
            return m_ConstantBufferDescs;
        }

      private:
        ShaderAsset::ConstRef m_pVertexShader;
        ShaderAsset::ConstRef m_pPixelShader;

        GraphicsPipeline::Ref m_pGraphicsPipeline;

        primitive::HashMap< primitive::String, rhi::BindingLayoutElement > m_Bindings;
        primitive::HashMap< primitive::String, ConstantBufferDescriptor > m_ConstantBufferDescs;
    };
}

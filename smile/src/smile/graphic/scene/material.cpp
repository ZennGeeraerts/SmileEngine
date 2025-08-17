/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "material.h"

#include "smile/graphic/renderer/render_engine.h"

namespace smile::graphic
{
    Material::Material( const ShaderAsset::ConstRef &pVertexShader, const ShaderAsset::ConstRef &pPixelShader )
    {
        SetShaders( pVertexShader, pPixelShader );
    }

    Material::~Material()
    {
    }

    void Material::SetShaders( const ShaderAsset::ConstRef &pVertexShader, const ShaderAsset::ConstRef &pPixelShader )
    {
        m_Bindings.Clear();
        m_ConstantBufferDescs.Clear();

        m_pVertexShader = pVertexShader;
        m_pPixelShader = pPixelShader;

        GraphicsPipelineDescriptor psoDesc{};
        psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
        psoDesc.InputLayout = GetBufferLayout();
        psoDesc.pVertexShader = pVertexShader->GetVertexShader();
        psoDesc.pPixelShader = pPixelShader->GetPixelShader();

        rhi::BindingLayout bindingLayout{ { rhi::ShaderStage::Vertex, rhi::ShaderStage::Pixel } };
        psoDesc.BindingLayouts.PushBack( std::move( bindingLayout ) );

        auto addBindingLayoutElements = [&]( const ShaderReflectionData &reflectionData )
        {
            for ( const auto &binding : reflectionData.ShaderResourceBindings )
            {
                SM_ASSERT_MSG( !m_Bindings.HasItemAtKey( binding.Key ),
                    "Bindings already contain an element with the name: {}",
                    binding.Key );

                psoDesc.BindingLayouts[0].AddElement( binding.Value );

                m_Bindings.Insert( binding.Key, binding.Value );

                if ( binding.Value.Type == rhi::ResourceType::ConstantBuffer )
                {
                    SM_ASSERT_MSG( !m_ConstantBufferDescs.HasItemAtKey( binding.Key ),
                        "Constant buffer descriptors already contain a descriptor with the name: {}",
                        binding.Key )

                    const auto &constantBufferDesc = reflectionData.ConstantBufferDescs.GetItemAtKey( binding.Key );
                    m_ConstantBufferDescs.Insert( binding.Key, std::move( constantBufferDesc ) );
                }
            }
        };

        addBindingLayoutElements( pVertexShader->GetReflectionData() );
        addBindingLayoutElements( pPixelShader->GetReflectionData() );

        m_pGraphicsPipeline = RenderEngine::GetRenderSystem().GetResourceManager().CreateGraphicsPipeline( psoDesc );
    }
}
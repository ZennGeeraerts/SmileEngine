/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "material.h"

#include "smile/graphic/renderer/render_engine.h"

namespace smile::graphic
{
    Material::Material( const ShaderAsset::Ref &pVertexShader, const ShaderAsset::Ref &pPixelShader )
        : m_BindingLayout{ { rhi::ShaderStage::Vertex, rhi::ShaderStage::Pixel } }
    {
        SetShaders( pVertexShader, pPixelShader );
    }

    Material::~Material()
    {
    }

    void Material::SetShaders( ShaderAsset::Ref pVertexShader, ShaderAsset::Ref pPixelShader )
    {
        m_pVertexShader = pVertexShader;
        m_pPixelShader = pPixelShader;

        Clear();

        auto insertResourcesFunc = [&]( const ShaderReflectionData &reflectionData )
        {
            for ( const ShaderResourceBinding &binding : reflectionData.ShaderResourceBindings )
            {
                rhi::BindingLayoutElement element{ binding.BindPoint, binding.Type };
                m_BindingLayout.AddElement( std::move( element ) );

                switch ( binding.Type )
                {
                    case rhi::ResourceType::Texture_SRV:
                    case rhi::ResourceType::Texture_UAV:
                        m_Textures.Insert( binding.Name, nullptr );
                        break;

                    case rhi::ResourceType::Sampler:
                        m_Samplers.Insert( binding.Name, nullptr );
                        break;

                    case rhi::ResourceType::ConstantBuffer:
                    {
                        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();

                        const auto &constantBufferDesc = reflectionData.ConstantBufferDescs.at( binding.Name );
                        auto pConstantBuffer = resourceManager.CreateConstantBuffer( constantBufferDesc );
                        m_ConstantBuffers.Insert( binding.Name, pConstantBuffer );

                        break;
                    }

                    default:
                        SM_ASSERT_MSG( false, "Unsupported resource type" );
                }
            }
        };

        insertResourcesFunc( pVertexShader->GetReflectionData() );
        insertResourcesFunc( pPixelShader->GetReflectionData() );
    }

    void Material::Clear()
    {
        m_BindingLayout.Clear();
        m_ConstantBuffers.Clear();
        m_Textures.Clear();
        m_Samplers.Clear();
    }

    void Material::SetTexture( const primitive::StringView name, Texture::Ref pNewTexture )
    {
        if ( !m_Textures.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetTexture > Couldn't find texture with name: {}", name );
            return;
        }

        auto &pTexture = m_Textures.GetItemAtKey( name );
        pTexture = pNewTexture;
    }

    void Material::SetSampler( const primitive::StringView name, Sampler::Ref pNewSampler )
    {
        if ( !m_Samplers.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetSampler > Couldn't find sampler with name: {}", name );
            return;
        }

        auto &pSampler = m_Samplers.GetItemAtKey( name );
        pSampler = pNewSampler;
    }
}
/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/shader/shader_asset.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/resource/sampler.h"
#include "smile/graphic/renderer/resource/texture.h"
#include "smile/graphic/rhi/shader/binding_layout.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/hash_map.h"

namespace smile::graphic
{
    class Material final
    {
      public:
        Material( const ShaderAsset::Ref &pVertexShader, const ShaderAsset::Ref &pPixelShader ) noexcept;
        ~Material() noexcept;

        void SetShaders( ShaderAsset::Ref pVertexShader, ShaderAsset::Ref pPixelShader );

        void Clear();

        inline const rhi::BufferLayout &GetBufferLayout() const
        {
            return m_pVertexShader->GetReflectionData().InputSignature;
        }

        inline VertexShader::Ref GetVertexShader() const
        {
            return m_pVertexShader->GetVertexShader();
        }

        inline PixelShader::Ref GetPixelShader() const
        {
            return m_pPixelShader->GetPixelShader();
        }

        inline Texture::Ref GetTexture( const primitive::StringView name ) const
        {
            return m_Textures.GetItemAtKey( name );
        }

        inline Sampler::Ref GetSampler( const primitive::StringView name ) const

        {
            return m_Samplers.GetItemAtKey( name );
        }

        inline ConstantBuffer::Ref GetConstantBuffer( const primitive::StringView name ) const
        {
            return m_ConstantBuffers.GetItemAtKey( name );
        }

        void SetTexture( const primitive::StringView name, Texture::Ref pNewTexture );
        void SetSampler( const primitive::StringView name, Sampler::Ref pNewSampler );

        template < typename ConstantBufferType >
        void SetConstantBuffer( const primitive::StringView name, ConstantBufferType *pBuffer )
        {
            if ( !m_ConstantBuffers.HasItemAtKey( name ) )
            {
                SM_LOG_WARNING( "Material::SetConstantBuffer > Couldn't find constant buffer with name: {}", name );
                return;
            }

            auto pConstantBuffer = m_ConstantBuffers.GetItemAtKey( name );
            pConstantBuffer->Initialize( pBuffer );
        }

        const primitive::HashMap< primitive::String, Texture::Ref > &GetTextures() const
        {
            return m_Textures;
        }

        const primitive::HashMap< primitive::String, Sampler::Ref > &GetSamplers() const
        {
            return m_Samplers;
        }

        const primitive::HashMap< primitive::String, ConstantBuffer::Ref > GetConstantBuffers() const
        {
            return m_ConstantBuffers;
        }

      private:
        ShaderAsset::Ref m_pVertexShader;
        ShaderAsset::Ref m_pPixelShader;

        rhi::BindingLayout m_BindingLayout;

        primitive::HashMap< primitive::String, Texture::Ref > m_Textures;
        primitive::HashMap< primitive::String, Sampler::Ref > m_Samplers;
        primitive::HashMap< primitive::String, ConstantBuffer::Ref > m_ConstantBuffers;
    };
}

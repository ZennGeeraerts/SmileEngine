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

#include <DirectXMath.h>

namespace smile::graphic
{
    enum class MaterialParamType
    {
        Texture,
        Sampler,
        Float,
        Int,
        Bool,
        Float2,
        Float3,
    };

    using MaterialParamValue = std::variant< primitive::Vector< Byte >, Texture::Ref, Sampler::Ref >;

    struct MaterialParam final
    {
        MaterialParam( const primitive::String &name, MaterialParamType type, const MaterialParamValue &value )
            : Name{ name }, Type{ type }, Data{ value }
        {
        }

        primitive::String Name;
        MaterialParamType Type;
        MaterialParamValue Data;
    };

    class Material final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Material >;
        using ConstRef = memory::Ref< const Material >;

        using ConstantBufferData = primitive::Vector< Byte >;

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

        inline const MaterialParam &GetParam( const primitive::StringView name ) const
        {
            return m_Params.GetItemAtKey( name );
        }

        inline const ConstantBufferData &GetConstantBufferData( const primitive::StringView name ) const
        {
            return m_ConstantBufferData.GetItemAtKey( name );
        }

        void SetParam( const primitive::StringView name, const MaterialParamValue &data );

      private:
        ShaderAsset::Ref m_pVertexShader;
        ShaderAsset::Ref m_pPixelShader;

        rhi::BindingLayout m_BindingLayout;

        primitive::HashMap< primitive::String, MaterialParam > m_Params;
        primitive::HashMap< primitive::String, ConstantBufferData > m_ConstantBufferData;
    };
}

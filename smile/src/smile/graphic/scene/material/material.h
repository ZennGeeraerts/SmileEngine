/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/shader/vertex_shader.h"
#include "smile/graphic/renderer/shader/pixel_shader.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/shader/binding_set.h"
#include "smile/graphic/renderer/resource/texture.h"
#include "smile/graphic/renderer/resource/sampler.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/hash_map.h"

#include <DirectXMath.h>

#include <variant>

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

    using MaterialParamValue = std::variant< bool,
        int,
        float,
        DirectX::XMFLOAT2,
        DirectX::XMFLOAT3,
        Texture::Ref,
        rhi::SamplerDescriptor,
        primitive::Vector< Byte > >;

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

        Material();

        void Clear();

        inline VertexShader::ConstRef GetVertexShader() const
        {
            return m_pVertexShader;
        }

        inline PixelShader::ConstRef GetPixelShader() const
        {
            return m_pPixelShader;
        }

        void SetParam( const primitive::StringView name, const MaterialParamValue &data );
        const MaterialParam &GetParam( const primitive::StringView name ) const;

      private:
        void SetConstantBufferData( ConstantBuffer::Ref constantBuffer,
            const primitive::StringView name,
            const MaterialParamValue &data );

        void SetBindingSetData( BindingSet::Ref bindingSet,
            const primitive::StringView name,
            const MaterialParamValue &data );

      private:
        VertexShader::ConstRef m_pVertexShader;
        PixelShader::ConstRef m_pPixelShader;

        primitive::HashMap< primitive::String, MaterialParam > m_Params;

        ConstantBuffer::Ref m_VertexShaderCB;
        ConstantBuffer::Ref m_PixelShaderCB;

        BindingSet::Ref m_VertexShaderBindings;
        BindingSet::Ref m_PixelShaderBindings;
    };
}

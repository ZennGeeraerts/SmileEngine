/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/shader/vertex_shader.h"
#include "smile/graphic/renderer/shader/pixel_shader.h"
#include "smile/graphic/renderer/resource/texture.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/handle.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/foundation/flags.h"

#include <DirectXMath.h>

#include <variant>

namespace smile::graphic
{
    struct MaterialParameter final
    {
        using Value = std::variant< bool, int, float, DirectX::XMFLOAT2, DirectX::XMFLOAT3, Texture::Ref >;

        enum class Type
        {
            Texture,
            Sampler,
            Float,
            Int,
            Bool,
            Float2,
            Float3,
        };

        MaterialParameter( const primitive::String &name, Type type, const Value &value )
            : Name{ name }, ParamType{ type }, Data{ value }
        {
        }

        primitive::String Name;
        Type ParamType;
        Value Data;

        inline bool operator==( const MaterialParameter &other ) const
        {
            return Name == other.Name && ParamType == other.ParamType && Data == other.Data;
        }
    };

    struct TextureBinding final
    {
        primitive::String Name;
        Texture::Ref Texture;

        inline bool operator==( const TextureBinding &other )
        {
            return Name == other.Name && Texture == other.Texture;
        }
    };

    struct MaterialDescriptor final
    {
        VertexShader::Ref VertexShader;
        PixelShader::Ref PixelShader;
        primitive::HashMap< primitive::String, MaterialParameter > Parameters;
        primitive::HashMap< primitive::String, TextureBinding > TextureBindings;
    };

    class Material final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Material >;
        using ConstRef = memory::Ref< const Material >;

        using ID = primitive::Handle< Uint32, 24, 8 >;

        Material( const MaterialDescriptor &desc );
        ~Material() = default;

        void Clear();

        VertexShader::ConstRef GetVertexShader() const
        {
            return m_Descriptor.VertexShader;
        }

        PixelShader::ConstRef GetPixelShader() const
        {
            return m_Descriptor.PixelShader;
        }

        void SetParameter( const primitive::StringView name, const MaterialParameter::Value &value );
        MaterialParameter::Value GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name, Texture::Ref texture );
        Texture::Ref GetTextureBinding( const primitive::StringView name ) const;

      private:
        enum class DirtyFlags
        {
            Parameter,
            Texture,
        };

        MaterialDescriptor m_Descriptor;
        ID m_ID;

        foundation::Flags< DirtyFlags > m_DirtyFlags;
    };
}

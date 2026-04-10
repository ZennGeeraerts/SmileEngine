/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/handle.h"
#include "smile/common/foundation/flags.h"
#include "smile/common/memory/counted.h"
#include "smile/graphic/renderer/shader/program.h"
#include "smile/graphic/renderer/resource/texture.h"
#include "smile/graphic/rhi/resource/sampler.h"

#include <DirectXMath.h>

#include <variant>

namespace smile::graphic
{
    class Material;

    using MaterialParameterValue = std::
        variant< bool, int, float, DirectX::XMFLOAT2, DirectX::XMFLOAT3, DirectX::XMFLOAT4, primitive::Vector< Byte > >;

    struct MaterialTextureBinding final
    {
        MaterialTextureBinding() = default;

        MaterialTextureBinding( Texture::ConstRef texture, const rhi::SamplerDescriptor &samplerDesc ) noexcept
            : Texture{ texture }, SamplerDescriptor{ samplerDesc }
        {
        }

        bool operator==( const MaterialTextureBinding &other ) const noexcept
        {
            return Texture == other.Texture && SamplerDescriptor == other.SamplerDescriptor;
        }

        bool operator!=( const MaterialTextureBinding &other ) const noexcept
        {
            return !( *this == other );
        }

        Texture::ConstRef Texture;
        rhi::SamplerDescriptor SamplerDescriptor;
    };

    struct MaterialDescriptor final
    {
        Program::ConstRef ShaderProgram;
        primitive::HashMap< primitive::String, MaterialParameterValue > Parameters;
        primitive::HashMap< primitive::String, MaterialTextureBinding > TextureBindings;
    };

    class MaterialInstance final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< MaterialInstance >;
        using ConstRef = memory::Ref< const MaterialInstance >;

        using ID = primitive::Handle< Uint32, 24, 8 >;

        enum class DirtyFlags
        {
            Parameter,
            Texture,
        };

        // TODO: Create Link for material instead of const ref
        MaterialInstance( ID id, const MaterialDescriptor &desc, const Material &material );
        ~MaterialInstance() = default;

        void Clear();

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );
        const MaterialParameterValue &GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name,
            Texture::ConstRef texture,
            const rhi::SamplerDescriptor &samplerDesc );

        const MaterialTextureBinding &GetTextureBinding( const primitive::StringView name ) const;

        const MaterialDescriptor &GetDescriptor() const
        {
            return m_Descriptor;
        }

        ID GetID() const
        {
            return m_ID;
        }

        const Material &GetMaterial() const
        {
            return m_Material;
        }

        foundation::Flags< DirtyFlags > GetDirtyFlags() const
        {
            return m_DirtyFlags;
        }

        void ClearDirtyFlags()
        {
            m_DirtyFlags.ClearAll();
        }

      private:
        MaterialDescriptor m_Descriptor;
        ID m_ID;
        const Material &m_Material;
        foundation::Flags< DirtyFlags > m_DirtyFlags;
    };
}

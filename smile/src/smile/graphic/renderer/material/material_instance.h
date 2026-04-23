/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "material_descriptor.h"

#include "smile/common/foundation/flags.h"
#include "smile/common/memory/counted.h"

namespace smile::graphic
{
    class Material;

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
            const Texture &texture,
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

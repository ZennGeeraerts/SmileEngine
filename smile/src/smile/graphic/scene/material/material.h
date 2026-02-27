/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "material_asset.h"
#include "smile/common/primitive/handle.h"
#include "smile/common/foundation/flags.h"
#include "smile/common/memory/counted.h"

namespace smile::graphic
{
    class Material final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Material >;
        using ConstRef = memory::Ref< const Material >;

        using ID = primitive::Handle< Uint32, 24, 8 >;

        enum class DirtyFlags
        {
            Parameter,
            Texture,
        };

        Material( ID id, const MaterialLayout &layout, const MaterialDescriptor &desc );
        ~Material() = default;

        void Clear();

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );
        MaterialParameterValue GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name, Texture::Ref texture );
        Texture::Ref GetTextureBinding( const primitive::StringView name ) const;

        const MaterialLayout &GetLayout() const
        {
            return m_Layout;
        }

        const MaterialDescriptor &GetDescriptor() const
        {
            return m_Descriptor;
        }

        ID GetID() const
        {
            return m_ID;
        }

        foundation::Flags< DirtyFlags > GetDirtyFlags() const
        {
            return m_DirtyFlags;
        }

      private:
        MaterialLayout m_Layout;
        MaterialDescriptor m_Descriptor;
        ID m_ID;

        foundation::Flags< DirtyFlags > m_DirtyFlags;
    };
}

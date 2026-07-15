/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        material_asset.h
 * @author      Zenn Geeraerts
 * @created     9 Januari 2026
 * @brief       Asset for material
 */
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/core/asset/asset.h"
#include "smile/graphic/renderer/material/material_layout.h"
#include "material_asset_descriptor.h"

namespace smile::graphic
{
    class MaterialAsset final : public asset::Asset< MaterialAsset >
    {
      public:
        using Ref = memory::Ref< MaterialAsset >;
        using ConstRef = memory::Ref< const MaterialAsset >;

        MaterialAsset() = default;

        MaterialAsset( const primitive::String &name,
            const MaterialLayout &layout,
            const MaterialAssetDescriptor &desc ) noexcept
            : m_Name{ name }, m_Descriptor{ desc }, m_Layout{ layout }
        {
        }

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );
        const MaterialParameterValue &GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name,
            TextureAsset::Ref texture,
            const rhi::SamplerDescriptor &samplerDesc );

        const MaterialAssetTextureBinding &GetTextureBinding( const primitive::StringView name ) const;

        primitive::StringView GetName() const
        {
            return m_Name.AsStringView();
        }

        const MaterialAssetDescriptor &GetDescriptor() const
        {
            return m_Descriptor;
        }

        const MaterialLayout &GetLayout() const
        {
            return m_Layout;
        }

      private:
        primitive::String m_Name;
        MaterialLayout m_Layout;
        MaterialAssetDescriptor m_Descriptor;

        friend class MaterialSerializer;
    };

    void BuildMaterialLayoutAndDescriptor( TextureAsset::Ref fallbackTexture,
        Program::ConstRef program,
        MaterialLayout &layout,
        MaterialAssetDescriptor &desc );
}
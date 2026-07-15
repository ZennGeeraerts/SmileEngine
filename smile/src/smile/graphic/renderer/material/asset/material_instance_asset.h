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
 * @file        material_instance_asset.h
 * @author      Zenn Geeraerts
 * @created     5 May 2026
 * @brief       Asset for material instance
 */
#pragma once

#include "material_asset.h"

namespace smile::graphic
{
    class MaterialInstanceAsset final : public asset::Asset< MaterialInstanceAsset >
    {
      public:
        using Ref = memory::Ref< MaterialInstanceAsset >;
        using ConstRef = memory::Ref< const MaterialInstanceAsset >;

        MaterialInstanceAsset() = default;

        MaterialInstanceAsset( const primitive::String &name,
            MaterialAsset::Ref materialAsset,
            const MaterialAssetDescriptor &desc ) noexcept
            : m_Name{ name }, m_MaterialAsset{ materialAsset }, m_Descriptor{ desc }
        {
        }

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );
        const MaterialParameterValue &GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name,
            TextureAsset::Ref texture,
            const rhi::SamplerDescriptor &samplerDesc );

        const MaterialAssetTextureBinding &GetTextureBinding( const primitive::StringView name ) const;

        primitive::StringView GetName() const noexcept
        {
            return m_Name.AsStringView();
        }

        MaterialAsset::Ref GetMaterialAsset() const noexcept
        {
            return m_MaterialAsset;
        }

        const MaterialAssetDescriptor &GetDescriptor() const noexcept
        {
            return m_Descriptor;
        }

      private:
        primitive::String m_Name;
        MaterialAsset::Ref m_MaterialAsset;
        MaterialAssetDescriptor m_Descriptor;

        friend class MaterialInstanceSerializer;
    };
}
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
    class MaterialInstanceAsset final : public asset::Asset
    {
      public:
        using Ref = memory::Ref< MaterialInstanceAsset >;
        using ConstRef = memory::Ref< const MaterialInstanceAsset >;

        MaterialInstanceAsset() = default;

        MaterialInstanceAsset( MaterialAsset::Ref materialAsset,
            const MaterialDescriptor &desc ) noexcept
            : m_MaterialAsset{ materialAsset }, m_Descriptor{ desc }
        {
        }

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );
        const MaterialParameterValue &GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name,
            const Texture &texture,
            const rhi::SamplerDescriptor &samplerDesc );

        const MaterialTextureBinding &GetTextureBinding( const primitive::StringView name ) const;

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< MaterialInstanceAsset >() };
        }

        const MaterialDescriptor &GetDescriptor() const
        {
            return m_Descriptor;
        }

      private:
        MaterialAsset::Ref m_MaterialAsset;
        MaterialDescriptor m_Descriptor;

        friend class MaterialInstanceSerializer;
    };
}
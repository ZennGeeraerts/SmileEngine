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
 * @file        material_instance_loader.h
 * @author      Zenn Geeraerts
 * @created     6 May 2026
 * @brief       Loads a material instance asset
 */
#pragma once

#include "smile/core/asset/asset_loader.h"
#include "material_instance_asset.h"

namespace smile::graphic
{
    class TextureManager;
    class ShaderLibrary;
    class MaterialManager;

    class MaterialInstanceLoader final : public asset::AssetLoader< MaterialInstanceAsset >
    {
      public:
        MaterialInstanceLoader( TextureManager &textureManager,
            ShaderLibrary &shaderLib,
            MaterialManager &materialManager ) noexcept;

        primitive::ArrayView< const fs::Path > GetExtensions() const override
        {
            return m_Extensions.AsView();
        }

        memory::Ref< asset::BaseAsset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        MaterialInstanceAsset::Ref Load( const fs::Path &path ) const override;

      private:
        const primitive::Array< fs::Path, 1 > m_Extensions{ ".smmati" };
        TextureManager &m_TextureManager;
        ShaderLibrary &m_ShaderLibrary;
        MaterialManager &m_MaterialManager;
    };
}
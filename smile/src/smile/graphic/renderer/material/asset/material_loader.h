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
 * @file        material_loader.h
 * @author      Zenn Geeraerts
 * @created     19 Januari 2026
 * @brief       Loads a material asset
 */
#pragma once
#include "smile/core/asset/asset_loader.h"
#include "material_asset.h"

namespace smile::graphic
{
    class TextureManager;
    class ShaderLibrary;

    class MaterialLoader final : public asset::AssetLoader
    {
      public:
        MaterialLoader( TextureManager &textureManager, ShaderLibrary &shaderLib ) noexcept;

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< MaterialAsset >() };
        }

        const std::vector< std::filesystem::path > &GetExtensions() const override
        {
            return m_Extensions;
        }

        memory::Ref< asset::Asset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        MaterialAsset::Ref LoadMaterial( const std::filesystem::path &path ) const;

      private:
        const std::vector< std::filesystem::path > m_Extensions{ ".smmat" };
        TextureManager &m_TextureManager;
        ShaderLibrary &m_ShaderLibrary;
    };
}
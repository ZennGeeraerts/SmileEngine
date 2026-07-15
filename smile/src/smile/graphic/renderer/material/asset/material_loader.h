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

#include "smile/common/primitive/collection/array.h"
#include "smile/core/asset/asset_loader.h"
#include "material_asset.h"

namespace smile::graphic
{
    class TextureManager;

    class MaterialLoader final : public asset::AssetLoader< MaterialAsset >
    {
      public:
        MaterialLoader( TextureManager &textureManager ) noexcept;

        primitive::ArrayView< const fs::Path > GetExtensions() const override
        {
            return m_Extensions.AsView();
        }

        memory::Ref< asset::BaseAsset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        MaterialAsset::Ref Load( const fs::Path &path ) const override;

      private:
        const primitive::Array< fs::Path, 1 > m_Extensions{ ".smmat" };
        TextureManager &m_TextureManager;
    };
}
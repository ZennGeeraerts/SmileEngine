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

#include "smile/core/asset/asset.h"
#include "material.h"

namespace smile::graphic
{
    class MaterialAsset final : public asset::Asset
    {
      public:
        MaterialAsset( Material::Ref pMaterial ) noexcept : m_pMaterial{ pMaterial }
        {
        }

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< MaterialAsset >() };
        }

        Material::Ref GetMaterial() const
        {
            return m_pMaterial;
        }

      private:
        Material::Ref m_pMaterial;
    };
}
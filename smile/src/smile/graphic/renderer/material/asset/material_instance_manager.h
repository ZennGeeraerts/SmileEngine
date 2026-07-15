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
 * @file        material_instance_manager.h
 * @author      Zenn Geeraerts
 * @created     6 May 2026
 * @brief       Manages material instance assets
 */
#pragma once

#include "smile/core/asset/asset_provider.h"
#include "material_instance_loader.h"

namespace smile::graphic
{
    class MaterialInstanceManager final
        : public asset::AssetProvider< MaterialInstanceAsset, primitive::String, MaterialInstanceLoader >
    {
      public:
        MaterialInstanceManager() = default;
        ~MaterialInstanceManager() = default;

        MaterialInstanceAsset::Ref CreateMaterialInstance( const primitive::String &name,
            MaterialAsset::Ref material,
            const MaterialAssetDescriptor &desc );

      protected:
        primitive::String GetKey( MaterialInstanceAsset::Ref asset ) const override
        {
            return asset->GetName();
        }
    };
}
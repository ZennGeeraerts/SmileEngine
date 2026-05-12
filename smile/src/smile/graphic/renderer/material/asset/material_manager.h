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
 * @file        material_manager.h
 * @author      Zenn Geeraerts
 * @created     27 February 2026
 * @brief       Manages material assets
 */
#pragma once

#include "smile/core/asset/asset_provider.h"
#include "material_loader.h"

namespace smile::graphic
{
    class MaterialManager final : public asset::AssetProvider< MaterialAsset, primitive::String, MaterialLoader >
    {
      public:
        MaterialManager( TextureManager &textureManager ) noexcept;
        ~MaterialManager() = default;

        MaterialAsset::Ref CreateMaterial( const primitive::String &name,
            const MaterialLayout &layout,
            const MaterialAssetDescriptor &desc );

        MaterialAsset::Ref CreateMaterial( const primitive::String &name, Program::ConstRef program );

        MaterialAsset::Ref CreateMaterial( const primitive::String &name,
            ShaderAsset::ConstRef vertexShader,
            ShaderAsset::ConstRef pixelShader );

      protected:
        primitive::String GetKey( MaterialAsset::Ref asset ) const override
        {
            return asset->GetName();
        }

      private:
        TextureManager &m_TextureManager;
    };
}
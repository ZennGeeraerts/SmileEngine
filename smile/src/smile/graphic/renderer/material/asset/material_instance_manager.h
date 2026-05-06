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

#include "material_instance_loader.h"

namespace smile::graphic
{
    class MaterialInstanceManager final
    {
      public:
        MaterialInstanceManager( TextureManager &textureManager,
            ShaderLibrary &shaderLibrary,
            MaterialManager &materialManagera ) noexcept;

        ~MaterialInstanceManager() = default;

        MaterialInstanceAsset::Ref CreateMaterialInstance( const primitive::String &name,
            MaterialAsset::Ref material,
            const MaterialDescriptor &desc );

        MaterialInstanceAsset::Ref GetMaterialInstance( asset::AssetHandle handle );
        MaterialInstanceAsset::Ref GetMaterialInstance( const primitive::StringView name ) const;

        MaterialInstanceAsset::Ref LoadMaterialInstance( const std::filesystem::path &path );

      private:
        MaterialInstanceLoader m_MaterialInstanceLoader;
        primitive::HashMap< primitive::String, MaterialInstanceAsset::Ref > m_MaterialInstances;
        TextureManager &m_TextureManager;
    };
}
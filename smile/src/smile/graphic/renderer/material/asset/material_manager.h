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

#include "material_loader.h"

namespace smile::graphic
{
    class MaterialManager final
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

        MaterialAsset::Ref GetMaterial( const primitive::StringView name ) const;

        MaterialAsset::Ref LoadMaterial( const fs::Path &path );

      private:
        MaterialLoader m_MaterialLoader;
        primitive::HashMap< primitive::String, MaterialAsset::Ref > m_Materials;
        TextureManager &m_TextureManager;
    };
}
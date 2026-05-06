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
 * @file        material_instance_serializer.h
 * @author      Zenn Geeraerts
 * @created     6 May 2026
 * @brief       Serializes a material instance asset to a file
 */
#pragma once

#include "material_instance_asset.h"
#include "smile/core/fs/path.h"

namespace smile::graphic
{
    class TextureManager;
    class ShaderLibrary;
    class MaterialManager;

    class MaterialInstanceSerializer final
    {
      public:
        MaterialInstanceSerializer( MaterialInstanceAsset::Ref materialInstanceAsset,
            TextureManager &textureManager,
            ShaderLibrary &shaderLib,
            MaterialManager &materialManager ) noexcept;

        void Serialize( const fs::Path &filePath ) const;
        bool Deserialize( const fs::Path &filePath );

      private:
        MaterialInstanceAsset::Ref m_MaterialInstanceAsset;
        TextureManager &m_TextureManager;
        ShaderLibrary &m_ShaderLib;
        MaterialManager &m_MaterialManager;
    };
}
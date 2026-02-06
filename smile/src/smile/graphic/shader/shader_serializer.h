/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        shader_serializer.h
 * @author      Zenn Geeraerts
 * @created     18 November 2025
 * @brief       Serialization and deserialization of CPU shaders
 */
#pragma once

#include "shader_asset.h"
#include "smile/core/fs/path.h"

namespace smile::graphic
{
    class ShaderSerializer final
    {
      public:
        ShaderSerializer( ShaderAsset::Ref pShaderAsset );

        void Serialize( const fs::Path &filePath ) const;
        bool Deserialize( const fs::Path &filePath );

      private:
        ShaderAsset::Ref m_pShaderAsset;
    };
}
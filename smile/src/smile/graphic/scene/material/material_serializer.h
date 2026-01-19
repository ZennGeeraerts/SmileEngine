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
 * @file        material_serializer.h
 * @author      Zenn Geeraerts
 * @created     16 Januari 2026
 * @brief       Serializes a material asset to a file
 */
#pragma once

#include "material_asset.h"
#include "smile/core/fs/path.h"

namespace smile::graphic
{
    class MaterialSerializer final
    {
      public:
        MaterialSerializer( MaterialAsset::Ref pMaterialAsset );

        void Serialize( const fs::Path &filePath ) const;
        bool Deserialize( const fs::Path &filePath );

      private:
        MaterialAsset::Ref m_pMaterialAsset;
    };
}
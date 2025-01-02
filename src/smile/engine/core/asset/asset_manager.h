/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset_manager_base.h"

#include "smile/engine/core/project/project_manager.h"

namespace smile::asset
{
    class AssetManager final
    {
      public:
        template < typename AssetType >
        static Ref< AssetType > GetAsset( AssetHandle handle )
        {
            Ref< Asset > pAsset = project::ProjectManager::GetActive()->GetAssetManager()->GetAsset( handle );
            return std::static_pointer_cast< AssetType >( pAsset );
        }
    };
}

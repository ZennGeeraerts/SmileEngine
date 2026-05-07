/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset_manager_base.h"

#include "smile/core/project/project_manager.h"

namespace smile::asset
{
    class AssetManager final
    {
      public:
        template < AssetDerived TAsset >
        static memory::Ref< TAsset > GetAsset( AssetHandle handle )
        {
            memory::Ref< BaseAsset > pAsset =
                project::ProjectManager::GetActive()->GetAssetManager()->GetAsset( handle );

            return memory::Ref< TAsset >{ pAsset };
        }
    };
}

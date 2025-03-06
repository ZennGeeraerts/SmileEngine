/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset_manager_base.h"
#include "asset_metadata.h"

namespace smile::asset
{
    using AssetRegistry = std::map< AssetHandle, AssetMetadata >;

    class EditorAssetManager : public AssetManagerBase
    {
      public:
        Ref< Asset > GetAsset( AssetHandle handle ) const override;

        bool IsAssetHandleValid( AssetHandle handle ) const override;
        bool IsAssetLoaded( AssetHandle handle ) const override;

        const AssetMetadata &GetMetadata( AssetHandle handle ) const;

      private:
        AssetRegistry m_AssetRegistry;
        AssetMap m_LoadedAssets;

        // TODO: memory-only assets
    };
}
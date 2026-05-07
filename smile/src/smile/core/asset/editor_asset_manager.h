/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset_manager_base.h"
#include "asset_metadata.h"
#include "smile/common/primitive/collection/dictionary.h"

namespace smile::asset
{
    using AssetRegistry = primitive::Dictionary< AssetHandle, AssetMetadata >;

    class EditorAssetManager : public AssetManagerBase
    {
      public:
        memory::Ref< Asset > GetAsset( AssetHandle handle ) override;

        bool IsAssetHandleValid( AssetHandle handle ) const override;
        bool IsAssetLoaded( AssetHandle handle ) const override;

        void ImportAsset( const fs::Path &path );

        const AssetMetadata &GetMetadata( AssetHandle handle ) const;
        const fs::Path &GetFilePath( AssetHandle handle ) const;

        const AssetRegistry &GetAssetRegistry() const
        {
            return m_AssetRegistry;
        }

        void SerializeAssetRegistry();
        bool DeserializeAssetRegistry();

      private:
        AssetRegistry m_AssetRegistry;
        AssetMap m_LoadedAssets;

        // TODO: memory-only assets
    };
}
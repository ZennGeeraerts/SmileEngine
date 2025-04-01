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
        memory::Ref< Asset > GetAsset( AssetHandle handle ) override;

        bool IsAssetHandleValid( AssetHandle handle ) const override;
        bool IsAssetLoaded( AssetHandle handle ) const override;

        void ImportAsset( const std::filesystem::path &path );

        const AssetMetadata &GetMetadata( AssetHandle handle ) const;
        const std::filesystem::path &GetFilePath( AssetHandle handle ) const;

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
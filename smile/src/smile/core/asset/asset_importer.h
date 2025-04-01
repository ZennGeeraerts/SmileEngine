/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset_metadata.h"
#include "smile/common/memory/ref.h"
#include "smile/common/foundation/meyers_singleton.h"

#include <map>

namespace smile::asset
{
    class AssetLoader;

    class AssetImporter final : public foundation::MeyersSingleton< AssetImporter >
    {
      public:
        void RegisterLoader( AssetLoader *pLoader );
        memory::Ref< Asset > ImportAsset( AssetHandle handle, const AssetMetadata &metadata );
        AssetType GetAssetTypeFromFileExtension( const std::filesystem::path &extension );

      private:
        std::map< AssetType, AssetLoader * > m_AssetLoaderMap{};
        std::map< std::filesystem::path, AssetType > m_AssetExtensionMap{};
    };
}
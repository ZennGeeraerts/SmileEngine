/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset_metadata.h"
#include "smile/common/memory/ref.h"
#include "smile/common/foundation/meyers_singleton.h"
#include "smile/common/primitive/collection/dictionary.h"
#include "smile/core/fs/path.h"

namespace smile::asset
{
    class BaseAssetLoader;

    class AssetImporter final : public foundation::MeyersSingleton< AssetImporter >
    {
      public:
        void RegisterLoader( BaseAssetLoader *pLoader );
        memory::Ref< Asset > ImportAsset( AssetHandle handle, const AssetMetadata &metadata );
        AssetType GetAssetTypeFromFileExtension( const fs::Path &extension );

      private:
        primitive::Dictionary< AssetType, BaseAssetLoader * > m_AssetLoaderMap{};
        primitive::Dictionary< fs::Path, AssetType > m_AssetExtensionMap{};
    };
}
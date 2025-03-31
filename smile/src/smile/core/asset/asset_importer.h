/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset_metadata.h"
#include "smile/common/memory/ref.h"

namespace smile::asset
{
    class AssetLoader;

	class AssetImporter
	{
      public:
        static void RegisterLoader( AssetLoader *pLoader );
        static memory::Ref< Asset > ImportAsset( AssetHandle handle, const AssetMetadata &metadata );
        static AssetType GetAssetTypeFromFileExtension( const std::filesystem::path &extension );
	};
}
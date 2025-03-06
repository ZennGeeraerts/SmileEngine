/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset.h"

#include <map>

namespace smile::asset
{
	using AssetMap = std::map< AssetHandle, Ref< Asset > >;

	class AssetManagerBase
	{
      public:
        virtual Ref< Asset > GetAsset( AssetHandle handle ) const = 0;

		virtual bool IsAssetHandleValid( AssetHandle handle ) const = 0;
        virtual bool IsAssetLoaded( AssetHandle handle ) const = 0;
	};
}

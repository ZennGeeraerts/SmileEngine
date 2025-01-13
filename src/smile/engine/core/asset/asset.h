/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/common/compiled/compiled.h"
#include "engine/common/primitive/uuid.h"

namespace smile::asset
{
	using AssetHandle = primitive::UUID;

	enum class AssetType : Uint16
	{
		None = 0,
		Scene,
		Model
    };

	class Asset
	{
      public:
        virtual AssetType GetType() const = 0;

      private:
        AssetHandle m_Handle;
	};
}

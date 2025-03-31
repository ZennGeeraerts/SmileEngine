/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset_metadata.h"
#include "smile/common/memory/ref.h"

namespace smile::asset
{
    class AssetLoader
    {
      public:
        virtual ~AssetLoader() = default;

        virtual AssetType GetType() const = 0;
        virtual const std::vector< std::filesystem::path > &GetExtensions() const = 0;

        virtual memory::Ref< Asset > Load( AssetHandle handle, const AssetMetadata &metadata ) const = 0;
    };
}
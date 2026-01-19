/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/dictionary.h"

namespace smile::asset
{
    using AssetMap = primitive::Dictionary< AssetHandle, memory::Ref< Asset > >;

    class AssetManagerBase
    {
      public:
        virtual memory::Ref< Asset > GetAsset( AssetHandle handle ) = 0;

        virtual bool IsAssetHandleValid( AssetHandle handle ) const = 0;
        virtual bool IsAssetLoaded( AssetHandle handle ) const = 0;
    };
}

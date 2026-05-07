/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset_metadata.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/array_view.h"
#include "smile/core/fs/path.h"

#include <type_traits>

namespace smile::asset
{
    class BaseAssetLoader
    {
      public:
        virtual ~BaseAssetLoader() = default;

        virtual AssetType GetType() const = 0;
        virtual primitive::ArrayView< const fs::Path > GetExtensions() const = 0;

        virtual memory::Ref< Asset > Load( AssetHandle handle, const AssetMetadata &metadata ) const = 0;
    };

    template < typename Type >
    concept AssetDerived = std::is_base_of_v< Asset, Type >;

    template < AssetDerived TAsset >
    class AssetLoader : public BaseAssetLoader
    {
      public:
        AssetType GetType() const override
        {
            return AssetType{ foundation::TypeNameOf< TAsset >() };
        }

        virtual memory::Ref< TAsset > Load( const fs::Path &path ) const = 0;
    };
}
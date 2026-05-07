/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/uuid.h"
#include "smile/common/memory/counted.h"
#include "asset_type.h"

#include <type_traits>

namespace smile::asset
{
    using AssetHandle = primitive::UUID;

    class BaseAsset : public memory::Counted
    {
      public:
        virtual AssetType GetType() const = 0;

        AssetHandle m_Handle;
    };

    template < typename Type >
    class Asset : public BaseAsset
    {
      public:
        AssetType GetType() const override
        {
            return AssetType{ foundation::TypeNameOf< Type >() };
        }
    };

    template < typename Type >
    concept AssetDerived = std::is_base_of_v< Asset< Type >, Type >;
}

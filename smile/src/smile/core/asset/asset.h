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
        BaseAsset( const AssetType type ) noexcept : m_Handle{ 0 }, m_Type{ type }
        {
        }

        AssetType GetType() const noexcept
        {
            return m_Type;
        }

        bool IsRegistered() const noexcept
        {
            return m_Handle != 0;
        }

        AssetHandle m_Handle;

      private:
        AssetType m_Type;
    };

    template < typename Type >
    class Asset : public BaseAsset
    {
      public:
        Asset() noexcept : BaseAsset( AssetType{ foundation::TypeNameOf< Type >() } )
        {
        }
    };

    template < typename Type >
    concept AssetDerived = std::is_base_of_v< Asset< Type >, Type >;
}

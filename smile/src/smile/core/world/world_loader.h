/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/array.h"
#include "smile/core/asset/asset_loader.h"
#include "world.h"

namespace smile::world
{
    class WorldLoader final : public asset::AssetLoader< World >
    {
      public:
        WorldLoader();

        primitive::ArrayView< const fs::Path > GetExtensions() const override
        {
            return m_Extensions.AsView();
        }

        memory::Ref< asset::BaseAsset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        World::Ref Load( const fs::Path &path ) const override;

        void SaveWorld( memory::Ref< World > pWorld, const fs::Path &path ) const;

      private:
        const primitive::Array< fs::Path, 1 > m_Extensions{ ".smile" };
    };
}
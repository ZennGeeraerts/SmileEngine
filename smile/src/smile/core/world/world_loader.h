/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/asset/asset_loader.h"

namespace smile::world
{
    class World;

    class WorldLoader final : public asset::AssetLoader
    {
      public:
        WorldLoader();

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< World >() };
        }

        const primitive::Vector< fs::Path > &GetExtensions() const override
        {
            return m_Extensions;
        }

        memory::Ref< asset::Asset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        memory::Ref< World > LoadWorld( const fs::Path &path ) const;
        void SaveWorld( memory::Ref< World > pWorld, const fs::Path &path ) const;

      private:
        const primitive::Vector< fs::Path > m_Extensions{ ".smile" };
    };
}
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
            return asset::AssetType{ std::string{ foundation::TypeNameOf< World >() } };
        }

        const std::vector< std::filesystem::path > &GetExtensions() const override
        {
            return m_Extensions;
        }

        memory::Ref< asset::Asset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        memory::Ref< World > LoadWorld( const std::filesystem::path &path ) const;
        void SaveWorld( memory::Ref< World > pWorld, const std::filesystem::path &path ) const;

      private:
        const std::vector< std::filesystem::path > m_Extensions{ ".smile" };
    };
}
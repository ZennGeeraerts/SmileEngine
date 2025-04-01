/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "asset.h"

namespace smile::asset
{
    std::string_view AssetTypeToString( AssetType type )
    {
        switch ( type )
        {
            case AssetType::None:
                return "AssetType::None";
            case AssetType::World:
                return "AssetType::World";
        }

        return "AssetType::<Invalid>";
    }

    AssetType AssetTypeFromString( std::string_view assetType )
    {
        if ( assetType == "AssetType::None" )
            return AssetType::None;
        else if ( assetType == "AssetType::World" )
            return AssetType::World;

        return AssetType::None;
    }
}
/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset.h"

#include <filesystem>

namespace smile::asset
{
    struct AssetMetadata final
    {
        AssetType Type = AssetType::NullType();
        std::filesystem::path FilePath;

        operator bool() const
        {
            return Type.IsValid();
        }
    };
}
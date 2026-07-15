/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "asset.h"
#include "smile/core/fs/path.h"

namespace smile::asset
{
    struct AssetMetadata final
    {
        AssetType Type = AssetType::NullType();
        fs::Path FilePath;

        operator bool() const
        {
            return Type.IsValid();
        }
    };
}
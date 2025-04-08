/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/uuid.h"
#include "smile/common/memory/counted.h"
#include "asset_type.h"

namespace smile::asset
{
    using AssetHandle = primitive::UUID;

    class Asset : public memory::Counted
    {
      public:
        virtual AssetType GetType() const = 0;

        AssetHandle m_Handle;
    };
}

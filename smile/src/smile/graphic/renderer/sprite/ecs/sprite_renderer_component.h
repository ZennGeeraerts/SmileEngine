/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/material/asset/material_instance_asset.h"
#include "smile/graphic/renderer/sprite/texture_asset.h"
#include "smile/common/memory/ref.h"

#include <DirectXMath.h>

namespace smile::graphic::ecs
{
    struct SpriteRendererComponent final
    {
        SpriteRendererComponent() = default;
        SpriteRendererComponent( const SpriteRendererComponent & ) = default;

        MaterialInstanceAsset::Ref Material;
        DirectX::XMFLOAT4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
        TextureAsset::Ref Texture;
    };
}
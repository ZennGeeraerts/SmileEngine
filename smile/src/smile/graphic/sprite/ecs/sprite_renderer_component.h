/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_api/resource/texture.h"
#include "memory/ref.h"

#include <DirectXMath.h>

namespace smile::graphic::ecs
{
    struct SpriteRendererComponent final
    {
        SpriteRendererComponent() = default;
        SpriteRendererComponent( const SpriteRendererComponent & ) = default;
        SpriteRendererComponent( const DirectX::XMFLOAT4 &color ) : Color{ color }
        {
        }

        DirectX::XMFLOAT4 Color = { 1.f, 1.f, 1.f, 1.f };
        memory::Ref< Texture > pTexture = nullptr;
    };
}
/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/common/foundation/singleton.h"
#include "engine/graphic/sprite/ecs/sprite_renderer_component.h"

namespace smile::graphic
{
    class Renderer2D final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        static void EndScene();
        static void OnRender();

        static void DrawQuad( const DirectX::XMFLOAT2 &position,
            float rotation,
            const DirectX::XMFLOAT2 &size,
            const DirectX::XMFLOAT4 &color );

        static void DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform,
            const graphic::ecs::SpriteRendererComponent &spriteRendererComponent );

        static void DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform, const DirectX::XMFLOAT4 &color );
        static void DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform,
            const Ref< Texture > &pTexture,
            const DirectX::XMFLOAT4 &color );
    };
}
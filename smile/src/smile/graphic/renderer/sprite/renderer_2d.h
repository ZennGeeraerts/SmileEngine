/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/meyers_singleton.h"

#include "smile/graphic/renderer/resource/frame_buffer.h"
#include "smile/graphic/renderer/material/material.h"

namespace smile::graphic
{
    class ResourceManager;
    class ShaderLibrary;
    class RenderScene;

    class Renderer2D final : public foundation::MeyersSingleton< Renderer2D >
    {
      public:
        void Initialize( ResourceManager &resourceManager, const ShaderLibrary &shaderLib );

        void ShutDown();

        /**
         * Sets the scene that DrawSprite() will submit renderables into this frame.
         * Must be called once per frame before any DrawSprite() calls.
         */
        void BeginFrame( RenderScene &scene );

        void DrawSprite( const DirectX::XMFLOAT2 &position,
            float rotation,
            const DirectX::XMFLOAT2 &size,
            MaterialInstance materialInstance );

        void DrawSprite( const DirectX::XMFLOAT4X4 &worldTransform, MaterialInstance materialInstance );
    };
}
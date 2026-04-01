/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/meyers_singleton.h"

#include "smile/graphic/renderer/view.h"
#include "smile/graphic/renderer/resource/frame_buffer.h"

namespace smile::graphic
{
    class Renderer2D final : public foundation::MeyersSingleton< Renderer2D >
    {
      public:
        void Initialize();
        void ShutDown();

        void BeginScene( const View &view );
        void EndScene();
        void OnRender( Framebuffer::Ref framebuffer );

        void DrawQuad( const DirectX::XMFLOAT2 &position,
            float rotation,
            const DirectX::XMFLOAT2 &size,
            const DirectX::XMFLOAT3 &color );

        void DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform, const DirectX::XMFLOAT3 &color );
        void DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform, Texture::ConstRef pTexture );
    };
}
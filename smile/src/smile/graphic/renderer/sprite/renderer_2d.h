/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/meyers_singleton.h"

#include "smile/graphic/renderer/resource/frame_buffer.h"
#include "smile/graphic/renderer/material/material_instance.h"

namespace smile::graphic
{
    class RenderContext;
    class ShaderLibrary;
    class ForwardRenderPass;

    class Renderer2D final : public foundation::MeyersSingleton< Renderer2D >
    {
      public:
        void Initialize( RenderContext &context, const ShaderLibrary &shaderLib, ForwardRenderPass *forwardRenderPass );
        void ShutDown();

        void DrawSprite( const DirectX::XMFLOAT2 &position,
            float rotation,
            const DirectX::XMFLOAT2 &size,
            MaterialInstance::Ref materialInstance );

        void DrawSprite( const DirectX::XMFLOAT4X4 &worldTransform, MaterialInstance::Ref materialInstance );
    };
}
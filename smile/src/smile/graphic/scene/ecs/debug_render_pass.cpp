/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "debug_render_pass.h"

#include "smile/graphic/renderer/debug_renderer.h"

namespace smile::graphic::ecs
{
    void DebugRenderPass::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto &debugRenderer = DebugRenderer::GetInstance();
        debugRenderer.BeginScene( camera, cameraTransform );

        debugRenderer.OnRender();

        debugRenderer.EndScene();
    }
}
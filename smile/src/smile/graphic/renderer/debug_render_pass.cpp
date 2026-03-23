/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "debug_render_pass.h"

#include "smile/graphic/renderer/debug_renderer.h"

namespace smile::graphic
{
    DebugRenderPass::DebugRenderPass()
    {
        DebugRenderer::CreateInstance();
        DebugRenderer::GetInstance().Initialize();
    }

    DebugRenderPass::~DebugRenderPass()
    {
        DebugRenderer::GetInstance().ShutDown();
        DebugRenderer::RemoveInstance();
    }

    void DebugRenderPass::OnRender( Framebuffer::Ref framebuffer,
        const Camera &camera,
        const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        DebugRenderer::GetInstance().OnRender( framebuffer, camera, cameraTransform );
    }
}
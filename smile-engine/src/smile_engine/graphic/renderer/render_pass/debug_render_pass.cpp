/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "debug_render_pass.h"

#include "smile_engine/graphic/renderer/debug_renderer.h"
#include "smile_engine/scene/components.h"

#include "smile_engine/physics/physics_engine.h"
#include "smile_engine/physics/physics_utils.h"

#include <PhysX/PxPhysicsAPI.h>

namespace smile::graphic
{
    void DebugRenderPass::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto &debugRenderer = DebugRenderer::GetInstance();
        debugRenderer.BeginScene( camera, cameraTransform );

        RenderPhysics();
        debugRenderer.OnRender();

        debugRenderer.EndScene();
    }

    void DebugRenderPass::OnRender( const EditorCamera &editorCamera )
    {
        auto &debugRenderer = DebugRenderer::GetInstance();
        debugRenderer.BeginScene( editorCamera );

        RenderPhysics();
        debugRenderer.OnRender();

        debugRenderer.EndScene();
    }

    void DebugRenderPass::RenderPhysics()
    {
        physx::PxScene *pPhysicsScene = physics::PhysicsEngine::GetScene();
        if ( !pPhysicsScene )
            return;

        const auto pxRenderBuffer = &pPhysicsScene->getRenderBuffer();
        const auto debugLines = pxRenderBuffer->getLines();

        for ( Uint32 i{}; i < pxRenderBuffer->getNbLines(); ++i )
        {
            const auto &line = debugLines[i];

            const DirectX::XMFLOAT3 start = physics::utils::ConvertToDirectXVector( line.pos0 );
            const DirectX::XMFLOAT3 end = physics::utils::ConvertToDirectXVector( line.pos1 );
            const auto colorStart = physics::utils::ConvertToDirectXColor( line.color0 );
            const auto colorEnd = physics::utils::ConvertToDirectXColor( line.color1 );

            DebugRenderer::GetInstance().SubmitLine( start, end, colorStart, colorEnd );
        }
    }
}
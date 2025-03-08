/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile/graphic/scene/ecs/physics_render_pass.h"

#include "smile/graphic/renderer/debug_renderer.h"
#include "smile/physics/physics_engine.h"

#include "platform/physx/physics/physx_utils.h"

#include <PxPhysicsAPI.h>

namespace smile::graphic::ecs
{
    void PhysicsRenderPass::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        const auto &pPhysicsWorlds = physics::PhysicsEngine::GetInstance().GetWorlds();

        for ( const Ref< physics::PhysicsWorld > &pWorld : pPhysicsWorlds )
        {
            if ( !pWorld->GetInternal() )
                continue;

            auto pPhysXScene = reinterpret_cast< physx::PxScene * >( pWorld->GetInternal() );

            const auto pRenderBuffer = &pPhysXScene->getRenderBuffer();
            const auto pDebugLines = pRenderBuffer->getLines();

            for ( Uint32 i{}; i < pRenderBuffer->getNbLines(); ++i )
            {
                const auto &line = pDebugLines[i];

                const DirectX::XMFLOAT3 start = physics::utils::ConvertToDirectXVector( line.pos0 );
                const DirectX::XMFLOAT3 end = physics::utils::ConvertToDirectXVector( line.pos1 );
                const auto colorStart = physics::utils::ConvertToDirectXColor( line.color0 );
                const auto colorEnd = physics::utils::ConvertToDirectXColor( line.color1 );

                graphic::DebugRenderer::GetInstance().SubmitLine( start, end, colorStart, colorEnd );
            }
        }
    }
}
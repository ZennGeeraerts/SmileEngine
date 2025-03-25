/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "graphic_system.h"

#include "smile/graphic/renderer/render_engine.h"

namespace smile::graphic::ecs
{
    void GraphicSystem::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        System::OnAdd( ecsEngine );

        memory::Ref< Scene > pScene = RenderEngine::GetSceneManager().GetScene( &ecsEngine );
        pScene->GetRenderPassList().OnAdd( ecsEngine );
    }

    void GraphicSystem::OnRemove( smile::ecs::ECSEngine &ecsEngine )
    {
        System::OnRemove( ecsEngine );

        memory::Ref< Scene > pScene = RenderEngine::GetSceneManager().GetScene( &ecsEngine );
        pScene->GetRenderPassList().OnRemove( ecsEngine );
    }

    void GraphicSystem::OnUpdate()
    {
        memory::Ref< Scene > pScene = RenderEngine::GetSceneManager().GetScene( m_pECSEngine );
        pScene->OnRender();
    }
}
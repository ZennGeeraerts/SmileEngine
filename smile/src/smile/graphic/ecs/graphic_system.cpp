/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "graphic_system.h"

#include "smile/graphic/renderer/render_engine.h"

namespace smile::graphic::ecs
{
    GraphicSystem::GraphicSystem()
    {
        RenderEngine::GetSceneManager().AddListener( this );
    }

    void GraphicSystem::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        System::OnAdd( ecsEngine );

        primitive::UUID worldID = world::WorldManager::GetActive()->GetUUID();
        memory::Ref< Scene > pScene = RenderEngine::GetSceneManager().GetScene( worldID );
        m_pScene->GetRenderPassList().OnAdd( ecsEngine );
    }

    void GraphicSystem::OnRemove( smile::ecs::ECSEngine &ecsEngine )
    {
        System::OnRemove( ecsEngine );

        primitive::UUID worldID = world::WorldManager::GetActive()->GetUUID();
        memory::Ref< Scene > pScene = RenderEngine::GetSceneManager().GetScene( worldID );
        m_pScene->GetRenderPassList().OnRemove( ecsEngine );
    }

    void GraphicSystem::OnUpdate()
    {
        primitive::UUID worldID = world::WorldManager::GetActive()->GetUUID();
        memory::Ref< Scene > pScene = RenderEngine::GetSceneManager().GetScene( worldID );
        m_pScene->OnRender();
    }

    void GraphicSystem::OnActiveSceneChanged( const memory::Ref< Scene > &pScene )
    {
        m_pScene = pScene;
    }
}
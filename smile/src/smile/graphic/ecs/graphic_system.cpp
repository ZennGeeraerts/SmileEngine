/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "graphic_system.h"

#include "smile/graphic/renderer/render_engine.h"

namespace smile::graphic::ecs
{
    void GraphicSystem::OnUpdate()
    {
        memory::Ref< Scene > pScene = RenderEngine::GetSceneManager().GetActive();
        pScene->OnRender();
    }
}
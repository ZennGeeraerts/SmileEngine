/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "graphic_system.h"

#include "engine/graphic/renderer/render_engine.h"

namespace smile::graphic::ecs
{
    void GraphicSystem::OnUpdate()
    {
        Ref< Scene > pScene = RenderEngine::GetScene();
        pScene->OnRender();
    }
}
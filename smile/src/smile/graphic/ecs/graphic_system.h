/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/ecs/system.h"
#include "smile/graphic/scene/scene_manager.h"

namespace smile::graphic::ecs
{
    class GraphicSystem final : public smile::ecs::System< GraphicSystem >, public SceneManager::Listener
    {
      public:
        GraphicSystem();

        void OnAdd( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnRemove( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnUpdate() override;

        void OnActiveSceneChanged( const memory::Ref< Scene > &pScene );

      private:
        memory::Ref< Scene > m_pScene = nullptr;
    };
}
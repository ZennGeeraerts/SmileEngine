/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/ecs/ecs_engine.h"
#include "smile_engine/graphic/camera/editor_camera.h"

namespace smile::graphic
{
    class RenderPass
    {
      public:
        RenderPass( ecs::ECSEngine &ecsEngine ) : m_ECSEngine{ ecsEngine }
        {
        }
        virtual ~RenderPass() = default;

        RenderPass( const RenderPass & ) = delete;
        RenderPass( RenderPass && ) = delete;
        RenderPass &operator=( const RenderPass & ) = delete;
        RenderPass &operator=( RenderPass && ) = delete;

        virtual void OnRender() = 0;
        virtual void OnRender( const EditorCamera &editorCamera ) = 0; 

      protected:
        ecs::ECSEngine &m_ECSEngine;
    };
}
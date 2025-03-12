/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/object.h"
#include "smile/core/ecs/ecs_engine.h"
#include "smile/graphic/camera/camera.h"

namespace smile::graphic::ecs
{
    class RenderPass : public memory::Object
    {
      public:
        RenderPass( smile::ecs::ECSEngine &ecsEngine ) : m_ECSEngine{ ecsEngine }
        {
        }
        virtual ~RenderPass() = default;

        RenderPass( const RenderPass & ) = delete;
        RenderPass( RenderPass && ) = delete;
        RenderPass &operator=( const RenderPass & ) = delete;
        RenderPass &operator=( RenderPass && ) = delete;

        virtual void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform ) = 0;

      protected:
        smile::ecs::ECSEngine &m_ECSEngine;
    };
}
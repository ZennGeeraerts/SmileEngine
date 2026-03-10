/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/core/ecs/ecs_engine.h"
#include "smile/graphic/renderer/camera.h"
#include "smile/graphic/renderer/resource/frame_buffer.h"

namespace smile::graphic::ecs
{
    class RenderPass : public memory::Counted
    {
      public:
        RenderPass() = default;
        virtual ~RenderPass() = default;

        RenderPass( const RenderPass & ) = delete;
        RenderPass( RenderPass && ) = delete;
        RenderPass &operator=( const RenderPass & ) = delete;
        RenderPass &operator=( RenderPass && ) = delete;

        virtual void OnAdd( smile::ecs::ECSEngine &ecsEngine )
        {
            m_pECSEngine = &ecsEngine;
        };

        virtual void OnRemove( smile::ecs::ECSEngine &ecsEngine )
        {
            SM_ASSERT_MSG( m_pECSEngine == &ecsEngine, "System does not belong to this ecs engine" );
            m_pECSEngine = nullptr;
        };

        virtual void
        OnRender( Framebuffer::Ref framebuffer, const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform ) = 0;

      protected:
        smile::ecs::ECSEngine *m_pECSEngine;
    };
}
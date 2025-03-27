/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_pass.h"

namespace smile::graphic::ecs
{
    class RenderPassList final
    {
      public:
        RenderPassList() = default;
        ~RenderPassList()
        {
            Clear();
        }

        void OnAdd( smile::ecs::ECSEngine &ecsEngine );
        void OnRemove( smile::ecs::ECSEngine &ecsEngine );

        void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );

        void Add( memory::Ref< RenderPass > pRenderPass );

        void Clear()
        {
            m_pRenderPasses.clear();
        }

        auto begin() const
        {
            return m_pRenderPasses.begin();
        }
        auto end() const
        {
            return m_pRenderPasses.end();
        }

      private:
        std::vector< memory::Ref< RenderPass > > m_pRenderPasses{};
        smile::ecs::ECSEngine *m_pECSEngine = nullptr;
    };
}
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

        void OnAdd( smile::ecs::ECSEngine &ecsEngine )
        {
            m_pECSEngine = &ecsEngine;

            for ( const auto &pRenderPass : m_pRenderPasses )
                pRenderPass->OnAdd( ecsEngine );
        }

        void OnRemove( smile::ecs::ECSEngine &ecsEngine )
        {
            for ( const auto &pRenderPass : m_pRenderPasses )
                pRenderPass->OnRemove( ecsEngine );

            m_pECSEngine = nullptr;
        }

        void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
        {
            for ( auto &pRenderPass : m_pRenderPasses )
                pRenderPass->OnRender( camera, cameraTransform );
        }

        void Add( memory::Ref< RenderPass > pRenderPass )
        {
            m_pRenderPasses.push_back( pRenderPass );

            if ( m_pECSEngine )
                pRenderPass->OnAdd( *m_pECSEngine );
        }

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
/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
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
            ClearRenderPasses();
        }

        void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
        {
            for ( auto pRenderPass : m_pRenderPasses )
                pRenderPass->OnRender( camera, cameraTransform );
        }

        void OnRender( const EditorCamera &editorCamera )
        {
            for ( auto pRenderPass : m_pRenderPasses )
                pRenderPass->OnRender( editorCamera );
        }

        void AddRenderPass( RenderPass *pRenderPass )
        {
            m_pRenderPasses.push_back( pRenderPass );
        }
        void RemoveRenderPass( RenderPass *pRenderPass )
        {
            m_pRenderPasses.erase( std::remove( m_pRenderPasses.begin(), m_pRenderPasses.end(), pRenderPass ) );
            SAFE_DELETE( pRenderPass );
        }
        void ClearRenderPasses()
        {
            for ( auto pRenderPass : m_pRenderPasses )
                SAFE_DELETE( pRenderPass );

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
        std::vector< RenderPass * > m_pRenderPasses{};
    };
}
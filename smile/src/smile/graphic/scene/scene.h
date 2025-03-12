/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "ecs/render_pass_list.h"
#include "smile/graphic/renderer_backend/resource/frame_buffer.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class Scene final : public memory::Object
    {
      public:
        Scene( const window::Window *pWindow );

        void AddRenderPass( memory::Ref< ecs::RenderPass > pRenderPass )
        {
            m_RenderPassList.Add( std::move( pRenderPass ) );
        }

        void ClearRenderPasses()
        {
            m_RenderPassList.Clear();
        }

        void OnRender();

        memory::Ref< Framebuffer > GetFramebuffer() const
        {
            return m_pFramebuffer;
        }

        void *GetFinalColor() const
        {
            return m_pFramebuffer->GetColor( 0 );
        }

      private:
        ecs::RenderPassList m_RenderPassList;
        memory::Ref< Framebuffer > m_pFramebuffer;
    };
}
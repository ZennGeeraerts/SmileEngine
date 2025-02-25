/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "ecs/render_pass_list.h"
#include "engine/graphic/renderer_api/resource/frame_buffer.h"

namespace smile::graphic
{
    class Scene final
    {
      public:
        Scene() = default;

        void AddRenderPass( Ref< ecs::RenderPass > pRenderPass )
        {
            m_RenderPassList.Add( pRenderPass );
        }

        void OnRender();

        Ref< Framebuffer > GetFramebuffer() const
        {
            return m_pFramebuffer;
        }

        void *GetFinalColor() const
        {
            return m_pFramebuffer->GetColor( 0 );
        }

      private:
        ecs::RenderPassList m_RenderPassList;
        Ref< Framebuffer > m_pFramebuffer;

        friend class SceneManager;
    };
}
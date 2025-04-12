/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/core/world/entity.h"
#include "ecs/render_pass_list.h"
#include "smile/graphic/renderer/resource/frame_buffer.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class Scene final : public memory::Counted
    {
      public:
        struct CameraData final
        {
            Camera *pCamera = nullptr;
            DirectX::XMFLOAT4X4 CameraTransform{};
        };

      public:
        Scene( const window::Window *pWindow );

        void OnAdd( smile::ecs::ECSEngine &ecsEngine );
        void OnRemove( smile::ecs::ECSEngine &ecsEngine );

        ecs::RenderPassList &GetRenderPassList()
        {
            return m_RenderPassList;
        }

        void OnRender();

        void *GetFinalColor() const;
        void OnViewportResize( Uint32 width, Uint32 height );

        Uint32 GetViewportWidth() const
        {
            return m_ViewportWidth;
        }
        Uint32 GetViewportHeight() const
        {
            return m_ViewportHeight;
        }

        void SetPrimaryCameraEntity( world::Entity entity )
        {
            m_PrimaryCameraEntity = entity;
        }

        void SetFallbackCameraData( const CameraData &cameraData )
        {
            m_FallbackCameraData = cameraData;
        }

        void SetRenderToSwapChain( bool renderToSwapChain )
        {
            m_RenderToSwapChain = renderToSwapChain;
        }

      private:
        ecs::RenderPassList m_RenderPassList;
        memory::Ref< Framebuffer > m_pFramebuffer;

        Uint32 m_ViewportWidth = 0;
        Uint32 m_ViewportHeight = 0;

        world::Entity m_PrimaryCameraEntity;
        CameraData m_FallbackCameraData;

        bool m_RenderToSwapChain = false; // TODO: Blit framebuffer texture to swapchain texture
    };
}
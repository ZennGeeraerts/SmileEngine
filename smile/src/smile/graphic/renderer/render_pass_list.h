/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/vector.h"
#include "render_pass.h"

namespace smile::graphic
{
    class RenderPassList final
    {
      public:
        RenderPassList() = default;
        ~RenderPassList();

        void OnRender( Framebuffer::Ref framebuffer, const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );

        void PushBack( memory::Ref< RenderPass > pRenderPass );
        void PushFront( memory::Ref< RenderPass > pRenderPass );
        void Remove( memory::Ref< RenderPass > pRenderPass );
        void Clear();

        auto begin() const
        {
            return m_pRenderPasses.begin();
        }

        auto end() const
        {
            return m_pRenderPasses.end();
        }

      private:
        primitive::Vector< memory::Ref< RenderPass > > m_pRenderPasses{};
    };
}
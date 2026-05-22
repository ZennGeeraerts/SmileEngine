/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        renderer_test_layer.h
 * @author      Zenn Geeraerts
 * @created     10 March 2026
 * @brief       Renderer test application layer
 */
#pragma once

#include "smile/core/application/layer.h"
#include "smile/core/window/events/application_event.h"

#include "smile/graphic/renderer/render_scene.h"
#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/resource/index_buffer.h"
#include "smile/graphic/renderer/material/material.h"
#include "smile/graphic/renderer/render_engine.h"

namespace smile::graphic
{
    class RendererTestLayer final : public application::Layer
    {
      public:
        RendererTestLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate( primitive::Timestep deltaTime ) override;
        void OnEvent( window::Event &event ) override;
        void OnImGuiRender() override;

      private:
        memory::Scope< RenderEngine > m_RenderEngine;
        Renderer *m_Renderer = nullptr;
        rhi::SwapChain *m_SwapChain = nullptr;
        RenderScene *m_Scene = nullptr;

        VertexBuffer m_VertexBuffer;
        IndexBuffer m_IndexBuffer;
        Material m_Material;
    };
}
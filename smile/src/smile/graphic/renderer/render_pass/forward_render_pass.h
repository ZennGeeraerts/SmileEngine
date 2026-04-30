/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_pass.h"
#include "smile/graphic/renderer/render_collector.h"
#include "smile/graphic/renderer/graphics_state.h"
#include "smile/graphic/renderer/material/material.h"
#include "smile/graphic/renderer/resource/graphics_pipeline.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/shader/binding_layout.h"
#include "smile/graphic/renderer/shader/binding_set.h"

namespace smile::graphic
{
    class RenderContext;
    class ResourceManager;
    class MaterialSystem;

    class ForwardRenderPass final : public RenderPass
    {
      public:
        ForwardRenderPass( RenderContext &context,
            ResourceManager &resourceManager,
            MaterialSystem &materialSystem ) noexcept;

        void Initialize() override;
        void ShutDown() override;

        void BeginPass( const View &view ) override;
        void Execute( const Framebuffer &framebuffer ) override;
        void EndPass() override;

        void Submit( const DrawItem &drawItem );
        void Submit( DrawItem &&drawItem );

      private:
        void SetupMaterial( MaterialInstance materialInstance, GraphicsState &graphicsState );

        primitive::HashMap< MaterialInstance, GraphicsPipeline >::Iterator CreatePipeline(
            MaterialInstance materialInstanceHandle );

        void ClearDrawList();

      private:
        RenderCollector m_RenderCollector;
        primitive::HashMap< MaterialInstance, GraphicsPipeline > m_Pipelines;
        ConstantBuffer m_CameraCB;
        ConstantBuffer m_PerObjectCB;
        BindingLayout m_BindingLayout;
        BindingSet m_BindingSet;

        ViewConstants m_ViewConstants{};

        RenderContext &m_Context;
        ResourceManager &m_ResourceManager;
        MaterialSystem &m_MaterialSystem;
    };
}
/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/application/layer.h"
#include "smile/core/window/events/application_event.h"

#include "smile/graphic/rhi/graphics_device.h"

namespace smile::graphic
{
    class RHITestLayer final : public application::Layer
    {
      public:
        RHITestLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate( primitive::Timestep deltaTime ) override;
        void OnEvent( window::Event &event ) override;
        void OnImGuiRender() override;

      private:
        bool OnWindowResize( window::WindowResizeEvent &e );

      private:
        Scope< rhi::GraphicsDevice > m_pDevice;
        rhi::CommandList *m_pImmediateCommandList;
        Scope< rhi::SwapChain > m_pSwapChain;

        rhi::TextureHandleManager m_TextureHandleManager;

        rhi::FramebufferHandleManager m_FramebufferManager;
        rhi::FramebufferHandle m_Framebuffer;
        rhi::FramebufferDescriptor m_FramebufferDesc{};
        rhi::FramebufferInfoExtented m_FramebufferInfo{};

        rhi::ShaderHandleManager m_ShaderHandleManager;
        rhi::ShaderHandle m_VertexShaderHandle;
        rhi::ShaderHandle m_PixelShaderHandle;

        rhi::GraphicsPipelineHandleManager m_PipelineHandleManager;
        rhi::GraphicsPipelineHandle m_PipelineHandle;

        rhi::GPUBufferHandleManager m_GPUBufferHandleManager;
        rhi::GPUBufferHandle m_CameraConstantBufferHandle;
        rhi::GPUBufferHandle m_PerObjectBufferHandle;

        rhi::BindingSetHandleManager m_BindingSetHandleManager;
        rhi::BindingSetHandle m_BindingSetHandle;

        rhi::GPUBufferHandle m_VertexBufferHandle;
        rhi::GPUBufferHandle m_IndexBufferHandle;
    };
}
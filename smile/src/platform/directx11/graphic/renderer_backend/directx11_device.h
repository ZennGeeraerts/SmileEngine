/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/graphic/renderer_backend/rhi.h"
#include "smile/graphic/renderer_backend/graphics_device.h"
#include "smile/graphic/renderer_backend/render_state.h"

#include "resource/directx11_buffer.h"
#include "resource/directx11_texture.h"
#include "resource/directx11_frame_buffer.h"
#include "resource/directx11_pipeline.h"
#include "resource/directx11_rasterizer_state.h"
#include "resource/directx11_depth_stencil_state.h"
#include "resource/directx11_sampler.h"

#include "shader/directx11_shader.h"
#include "shader/directx11_input_layout.h"
#include "shader/directx11_binding_set.h"

#include "directx11_context.h"
#include "directx11_state_cache.h"

#include "smile/common/primitive/collection/array.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class DirectX11CommandList;

    class DirectX11Device final : public GraphicsDevice
    {
        using DirectX11RasterizerStateCache = typename DirectX11StateCache< RasterizerState, DirectX11RasterizerState >;
        using DirectX11DepthStencilStateCache =
            typename DirectX11StateCache< DepthStencilState, DirectX11DepthStencilState >;
        using DirectX11InputLayoutCache = typename DirectX11StateCache< BufferLayout, DirectX11InputLayout >;

      public:
        DirectX11Device();
        ~DirectX11Device();

        DirectX11Device( const DirectX11Device & ) = delete;
        DirectX11Device( DirectX11Device && ) = delete;
        DirectX11Device &operator=( const DirectX11Device & ) = delete;
        DirectX11Device &operator=( DirectX11Device && ) = delete;

        void *GetInternal() const override
        {
            return m_Context.pDevice;
        }

        CommandList *CreateCommandList() override;

        memory::Ref< SwapChain > CreateSwapChain( const window::Window *pWindow ) override;

        void CreateGPUBuffer( GPUBufferHandle handle,
            const GPUBufferDescriptor &bufferDesc,
            void *pData = nullptr ) override;
        void DestroyGPUBuffer( GPUBufferHandle handle ) override;

        void CreateBindingSet( BindingSetHandle handle,
            const BindingSetDescriptor &bindingSetDesc,
            const BindingLayout &layout ) override;
        void DestroyBindingSet( BindingSetHandle handle ) override;

        void CreateShader( ShaderHandle handle,
            const ShaderDescriptor &shaderDesc,
            const std::vector< Byte > &byteCode ) override;
        void DestroyShader( ShaderHandle handle ) override;

        void CreateGraphicsPipeline( GraphicsPipelineHandle handle,
            const GraphicsPipelineDescriptor &pipelineDesc ) override;
        void DestroyGraphicsPipeline( GraphicsPipelineHandle handle ) override;

        void CreateTexture( TextureHandle handle,
            const TextureDescriptor &desc,
            const std::vector< Byte > &buffer = {} ) override;
        void DestroyTexture( TextureHandle handle ) override;

        void CreateSampler( SamplerHandle handle, const SamplerDescriptor &samplerDesc ) override;
        void DestroySampler( SamplerHandle handle ) override;

        void CreateFramebuffer( FramebufferHandle handle, const FramebufferDescriptor &descriptor ) override;
        void DestroyFramebuffer( FramebufferHandle handle ) override;
        void InvalidateFramebuffer( FramebufferHandle handle ) override;

      private:
        const DirectX11RasterizerState *GetOrCreateRasterizerState( const RasterizerState &rasterizerState );
        const DirectX11DepthStencilState *GetOrCreateDepthStencilState( const DepthStencilState &depthStencilState );
        const DirectX11InputLayout *GetOrCreateInputLayout( const GraphicsPipelineDescriptor &pipelineDesc );

      private:
        DirectX11Context m_Context{};
        Scope< DirectX11CommandList > m_pImmediateCommandList;

        primitive::Array< DirectX11Buffer, s_MaxBufferCount > m_GPUBuffers;
        primitive::Array< DirectX11Texture, s_MaxTextureCount > m_Textures;
        primitive::Array< DirectX11Sampler, s_MaxSamplerCount > m_Samplers;
        primitive::Array< DirectX11Framebuffer, s_MaxFramebufferCount > m_Framebuffers;
        primitive::Array< DirectX11BindingSet, s_MaxBindingSetCount > m_BindingSets;
        primitive::Array< DirectX11Shader, s_MaxShaderCount > m_Shaders;
        primitive::Array< DirectX11Pipeline, s_MaxGraphicsPipelineCount > m_Pipelines;

        DirectX11RasterizerStateCache m_RasterizerStateCache;
        DirectX11DepthStencilStateCache m_DepthStencilStateCache;
        DirectX11InputLayoutCache m_InputLayoutCache;

        friend class DirectX11CommandList;
        friend class DirectX11BindingSet;
        friend class DirectX11Pipeline;
    };
}

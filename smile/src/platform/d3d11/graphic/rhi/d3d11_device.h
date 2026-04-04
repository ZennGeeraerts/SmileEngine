/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/graphic/rhi/rhi.h"
#include "smile/graphic/rhi/graphics_device.h"
#include "smile/graphic/rhi/render_state.h"

#include "resource/d3d11_buffer.h"
#include "resource/d3d11_texture.h"
#include "resource/d3d11_staging_texture.h"
#include "resource/d3d11_frame_buffer.h"
#include "resource/d3d11_pipeline.h"
#include "resource/d3d11_rasterizer_state.h"
#include "resource/d3d11_depth_stencil_state.h"
#include "resource/d3d11_sampler.h"

#include "shader/d3d11_shader.h"
#include "shader/d3d11_input_layout.h"
#include "shader/d3d11_binding_set.h"

#include "d3d11_context.h"
#include "d3d11_state_cache.h"

#include "smile/common/primitive/collection/array.h"

namespace smile::window
{
    class Window;
}

class ID3D11RenderTargetView;
class ID3D11DepthStencilView;

namespace smile::graphic::rhi
{
    class D3D11CommandList;

    class D3D11Device final : public GraphicsDevice
    {
        using D3D11RasterizerStateCache = typename D3D11StateCache< RasterizerState, D3D11RasterizerState >;
        using D3D11DepthStencilStateCache = typename D3D11StateCache< DepthStencilState, D3D11DepthStencilState >;
        using D3D11InputLayoutCache = typename D3D11StateCache< BufferLayout, D3D11InputLayout >;

      public:
        D3D11Device();
        ~D3D11Device();

        D3D11Device( const D3D11Device & ) = delete;
        D3D11Device( D3D11Device && ) = delete;
        D3D11Device &operator=( const D3D11Device & ) = delete;
        D3D11Device &operator=( D3D11Device && ) = delete;

        Object GetInternal( ObjectType type ) const override;

        CommandList *CreateCommandList() override;

        Scope< SwapChain > CreateSwapChain( const window::Window *pWindow ) override;

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
            const primitive::Vector< Byte > &byteCode ) override;
        void DestroyShader( ShaderHandle handle ) override;

        void CreateGraphicsPipeline( GraphicsPipelineHandle handle,
            const GraphicsPipelineDescriptor &pipelineDesc ) override;
        void DestroyGraphicsPipeline( GraphicsPipelineHandle handle ) override;

        void CreateTexture( TextureHandle handle,
            const TextureDescriptor &desc,
            const primitive::Vector< Byte > &buffer = {} ) override;
        void DestroyTexture( TextureHandle handle ) override;

        void CreateStagingTexture( StagingTextureHandle handle, const TextureDescriptor &desc ) override;
        void DestroyStagingTexture( StagingTextureHandle handle ) override;
        void *
        MapStagingTexture( StagingTextureHandle handle, const TextureSlice &slice, CPUAccessMode cpuAccess ) override;
        void UnmapStagingTexture( StagingTextureHandle handle ) override;

        void CreateHandleForNativeTexture( TextureHandle handle,
            Object nativeTexture,
            ObjectType type,
            const TextureDescriptor &desc ) override;

        Object GetNativeView( TextureHandle handle,
            ObjectType type,
            Format format,
            const TextureSubresourceSet &subresources,
            TextureDimension dimension,
            bool isReadOnlyDSV = false ) override;

        void CreateSampler( SamplerHandle handle, const SamplerDescriptor &samplerDesc ) override;
        void DestroySampler( SamplerHandle handle ) override;

        void CreateFramebuffer( FramebufferHandle handle, const FramebufferDescriptor &descriptor ) override;
        void DestroyFramebuffer( FramebufferHandle handle ) override;

      private:
        const D3D11RasterizerState *GetOrCreateRasterizerState( const RasterizerState &rasterizerState );
        const D3D11DepthStencilState *GetOrCreateDepthStencilState( const DepthStencilState &depthStencilState );
        const D3D11InputLayout *GetOrCreateInputLayout( const GraphicsPipelineDescriptor &pipelineDesc );

        ID3D11RenderTargetView *GetOrCreateRenderTargetViewForAttachment( const FramebufferAttachment &attachment );
        ID3D11DepthStencilView *GetOrCreateDepthStencilViewForAttachment( const FramebufferAttachment &attachment );

      private:
        D3D11Context m_Context{};
        Scope< D3D11CommandList > m_pImmediateCommandList;

        primitive::Array< D3D11Buffer, s_MaxBufferCount > m_GPUBuffers;
        primitive::Array< D3D11Texture, s_MaxTextureCount > m_Textures;
        primitive::Array< D3D11StagingTexture, s_MaxTextureCount > m_StagingTextures;
        primitive::Array< D3D11Sampler, s_MaxSamplerCount > m_Samplers;
        primitive::Array< D3D11Framebuffer, s_MaxFramebufferCount > m_Framebuffers;
        primitive::Array< D3D11BindingSet, s_MaxBindingSetCount > m_BindingSets;
        primitive::Array< D3D11Shader, s_MaxShaderCount > m_Shaders;
        primitive::Array< D3D11Pipeline, s_MaxGraphicsPipelineCount > m_Pipelines;

        D3D11RasterizerStateCache m_RasterizerStateCache;
        D3D11DepthStencilStateCache m_DepthStencilStateCache;
        D3D11InputLayoutCache m_InputLayoutCache;

        friend class D3D11CommandList;
        friend class D3D11BindingSet;
        friend class D3D11Pipeline;
        friend class D3D11Framebuffer;
    };
}

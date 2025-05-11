/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "command_list.h"
#include "render_handle.h"

#include "resource/buffer.h"
#include "resource/frame_buffer.h"
#include "resource/sampler.h"
#include "resource/graphics_pipeline.h"
#include "shader/shader.h"

#include "smile/graphic/resource/image.h"

#include <filesystem>

namespace smile::graphic
{
    enum class RendererBackendType
    {
        None = 0,
        DirectX11 = 1
    };

    class GraphicsDevice
    {
      public:
        GraphicsDevice() = default;
        virtual ~GraphicsDevice() = default;

        virtual void *GetInternal() const = 0;

        virtual CommandList *CreateCommandList() = 0;

        virtual memory::Ref< SwapChain > CreateSwapChain( const window::Window *pWindow ) = 0;

        virtual void CreateGPUBuffer( GPUBufferHandle handle, const GPUBufferDescriptor &bufferDesc ) = 0;
        virtual void DestroyGPUBuffer( GPUBufferHandle handle ) = 0;

        virtual void CreateShader( ShaderHandle handle,
            const ShaderDescriptor &shaderDesc,
            const std::vector< Byte > &byteCode ) = 0;
        virtual void DestroyShader( ShaderHandle handle ) = 0;

        virtual void CreateGraphicsPipeline( GraphicsPipelineHandle handle,
            const GraphicsPipelineDescriptor &pipelineDesc ) = 0;
        virtual void DestroyGraphicsPipeline( GraphicsPipelineHandle handle ) = 0;

        virtual void CreateTexture( TextureHandle handle, const std::filesystem::path &path ) = 0;
        virtual void CreateTexture( TextureHandle handle, memory::Ref< const Image > pImage ) = 0;
        virtual void DestroyTexture( TextureHandle handle ) = 0;

        virtual void CreateSampler( SamplerHandle handle, const SamplerDescriptor &samplerDesc ) = 0;
        virtual void DestroySampler( SamplerHandle handle ) = 0;

        virtual void CreateFramebuffer( FramebufferHandle handle, const FramebufferDescriptor &descriptor ) = 0;
        virtual void DestroyFramebuffer( FramebufferHandle handle ) = 0;
        virtual void InvalidateFramebuffer( FramebufferHandle handle ) = 0;

        static Scope< GraphicsDevice > Create( RendererBackendType backendType );

      protected:
        static constexpr Uint16 s_MaxBufferCount = ( 12 << 10 );
        static constexpr Uint16 s_MaxTextureCount = ( 4 << 10 );
        static constexpr Uint16 s_MaxSamplerCount = ( 4 << 10 );
        static constexpr Uint16 s_MaxFramebufferCount = ( 4 << 10 );
        static constexpr Uint16 s_MaxShaderCount = ( 4 << 10 );
        static constexpr Uint16 s_MaxGraphicsPipelineCount = ( 4 << 10 );
    };
}
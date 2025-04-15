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

    struct GPUBufferDescriptor final
    {
        void *pData = nullptr;
        Uint32 Size;
        BufferUsage Usage = BufferUsage::Default;
        BufferCPUAccess CPUAccess = BufferCPUAccess::None;
        BufferBindFlags BindFlags = BufferBindFlags::None;
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

        virtual memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const VertexLayout &layout,
            const std::string &techniqueName = "" ) = 0;
        virtual memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const std::string &techniqueName = "" ) = 0;

        virtual void CreateTexture( TextureHandle handle, const std::filesystem::path &path ) = 0;
        virtual void CreateTexture( TextureHandle handle, memory::Ref< const Image > pImage ) = 0;
        virtual void DestroyTexture( TextureHandle handle ) = 0;

        virtual void CreateFramebuffer( FramebufferHandle handle, const FramebufferDescriptor &descriptor ) = 0;
        virtual void DestroyFramebuffer( FramebufferHandle handle ) = 0;
        virtual void InvalidateFramebuffer( FramebufferHandle handle ) = 0;

        static Scope< GraphicsDevice > Create( RendererBackendType backendType );

      protected:
        static constexpr Uint16 s_MaxBufferCount = ( 12 << 10 );
        static constexpr Uint16 s_MaxTextureCount = ( 4 << 10 );
        static constexpr Uint16 s_MaxFramebufferCount = ( 4 << 10 );
    };
}
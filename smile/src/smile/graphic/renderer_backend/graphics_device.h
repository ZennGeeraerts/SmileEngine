/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "resource/swap_chain.h"
#include "resource/texture.h"
#include "resource/frame_buffer.h"
#include "resource/rasterizer_state.h"
#include "shader/shader.h"

#include "graphics_context.h"

#include "render_handle.h"

#include "memory/ref.h"

namespace smile::graphic
{
    enum class RendererBackendType
    {
        None = 0,
        DirectX11 = 1
    };

    struct VertexBufferDescriptor final
    {
        void *pVertices = nullptr;
        Uint32 Count = 0;
        Uint32 Stride = 0;
        BufferUsage Usage = BufferUsage::Default;
        BufferCPUAccess CPUAccess = BufferCPUAccess::None;
    };

    struct IndexBufferDescriptor final
    {
        Uint32 *pIndices = nullptr;
        Uint32 Count = 0;
        BufferUsage Usage = BufferUsage::Default;
    };

    class GraphicsDevice
    {
      public:
        GraphicsDevice() = default;
        virtual ~GraphicsDevice() = default;

        virtual void *GetInternal() const = 0;

        virtual GraphicsContext *CreateGraphicsContext() = 0;

        virtual memory::Ref< SwapChain > CreateSwapChain( const window::Window *pWindow ) = 0;
        virtual void
        ResizeBackBuffer( memory::Ref< SwapChain > pSwapChain, Uint32 x, Uint32 y, Uint32 width, Uint32 height ) = 0;

        virtual void CreateVertexBuffer( VertexBufferHandle handle,
            const VertexBufferDescriptor &vertexBufferDesc ) = 0;
        virtual void DestroyVertexBuffer( VertexBufferHandle handle ) = 0;

        virtual void CreateIndexBuffer( IndexBufferHandle handle, const IndexBufferDescriptor &indexBufferDesc ) = 0;
        virtual void DestroyIndexBuffer( IndexBufferHandle handle ) = 0;

        virtual memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const BufferLayout &layout,
            const std::string &techniqueName = "" ) = 0;
        virtual memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const std::string &techniqueName = "" ) = 0;
        virtual memory::Ref< Texture > CreateTexture2D( const std::string &filePath ) = 0;
        virtual memory::Ref< Texture > CreateTextureCube( const std::string &filePath ) = 0;
        virtual memory::Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor ) = 0;
        virtual memory::Ref< RasterizerState > CreateRasterizerState( const RasterizerStateDescriptor &descriptor ) = 0;

        virtual void InvalidateFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) = 0;

        static GraphicsDevice *Create( RendererBackendType backendType );

      protected:
        static constexpr Uint16 s_MaxVertexBufferSize = ( 4 << 10 );
        static constexpr Uint16 s_MaxIndexBufferSize = ( 4 << 10 );
    };
}
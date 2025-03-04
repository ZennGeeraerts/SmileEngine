/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/renderer_backend/graphics_device.h"
#include "resource/directx11_buffer.h"

#include <d3d11.h>

#include <array>

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class DirectX11Context;

    class DirectX11Device final : public GraphicsDevice
    {
      public:
        DirectX11Device();
        ~DirectX11Device();

        void *GetInternal() const override
        {
            return m_pInternal;
        }

        GraphicsContext *CreateGraphicsContext() override;

        memory::Ref< SwapChain > CreateSwapChain( const window::Window *pWindow ) override;
        void ResizeBackBuffer( memory::Ref< SwapChain > pSwapChain,
            Uint32 x,
            Uint32 y,
            Uint32 width,
            Uint32 height ) override;

        void CreateVertexBuffer( VertexBufferHandle handle, const VertexBufferDescriptor &vertexBufferDesc ) override;
        void DestroyVertexBuffer( VertexBufferHandle handle ) override;

        void CreateIndexBuffer( IndexBufferHandle handle, const IndexBufferDescriptor &indexBufferDesc ) override;
        void DestroyIndexBuffer( IndexBufferHandle handle ) override;

        memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const BufferLayout &layout,
            const std::string &techniqueName = "" ) override;
        memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const std::string &techniqueName = "" ) override;
        memory::Ref< Texture > CreateTexture2D( const std::string &filePath ) override;
        memory::Ref< Texture > CreateTextureCube( const std::string &filePath ) override;
        memory::Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor ) override;
        memory::Ref< RasterizerState > CreateRasterizerState( const RasterizerStateDescriptor &descriptor ) override;

        void InvalidateFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) override;

      private:
        ID3D11Device *m_pInternal = nullptr;
        ID3D11DeviceContext *m_pContext = nullptr;
        IDXGIFactory *m_pDXGIFactory = nullptr;

        std::vector< DirectX11Context * > m_pGraphicsContexts;

        std::array< DirectX11VertexBuffer, s_MaxVertexBufferSize > m_VertexBuffers;
        std::array< DirectX11IndexBuffer, s_MaxIndexBufferSize > m_IndexBuffers;

        friend class DirectX11Context;
    };
}

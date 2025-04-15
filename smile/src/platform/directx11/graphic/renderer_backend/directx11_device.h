/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/graphic/renderer_backend/graphics_device.h"
#include "smile/graphic/renderer_backend/render_state.h"
#include "resource/directx11_buffer.h"
#include "resource/directx11_texture.h"
#include "resource/directx11_frame_buffer.h"
#include "directx11_rasterizer_state_cache.h"
#include "directx11_depth_stencil_state_cache.h"
#include "directx11_sampler_state_cache.h"

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

        void CreateGPUBuffer( GPUBufferHandle handle, const GPUBufferDescriptor &bufferDesc ) override;
        void DestroyGPUBuffer( GPUBufferHandle handle ) override;

        memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const VertexLayout &layout,
            const std::string &techniqueName = "" ) override;
        memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const std::string &techniqueName = "" ) override;

        void CreateTexture( TextureHandle handle, const std::filesystem::path &path ) override;
        void CreateTexture( TextureHandle handle, memory::Ref< const Image > pImage ) override;
        void DestroyTexture( TextureHandle handle ) override;

        void CreateFramebuffer( FramebufferHandle handle, const FramebufferDescriptor &descriptor ) override;
        void DestroyFramebuffer( FramebufferHandle handle ) override;
        void InvalidateFramebuffer( FramebufferHandle handle ) override;

        const DirectX11RasterizerState *GetOrCreateRasterizerState( const RenderState &renderState );
        const DirectX11DepthStencilState *GetOrCreateDepthStencilState( const RenderState &renderState );
        const DirectX11SamplerState *GetOrCreateSamplerState( const SamplerState &samplerState );

      private:
        ID3D11Device *m_pInternal = nullptr;
        ID3D11DeviceContext *m_pContext = nullptr;
        IDXGIFactory *m_pDXGIFactory = nullptr;

        std::vector< Scope< DirectX11Context > > m_pGraphicsContexts;

        std::array< DirectX11Buffer, s_MaxBufferCount > m_GPUBuffers;
        std::array< DirectX11Texture, s_MaxTextureCount > m_Textures;
        std::array< DirectX11Framebuffer, s_MaxFramebufferCount > m_Framebuffers;

        DirectX11RasterizerStateCache m_RasterizerStateCache;
        DirectX11DepthStencilStateCache m_DepthStencilStateCache;
        DirectX11SamplerStateCache m_SamplerStateCache;

        friend class DirectX11Context;
        friend class DirectX11Shader;
    };
}

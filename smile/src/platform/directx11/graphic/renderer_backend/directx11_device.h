/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/graphic/renderer_backend/graphics_device.h"
#include "smile/graphic/renderer_backend/render_state.h"
#include "resource/directx11_buffer.h"
#include "resource/directx11_rasterizer_state.h"

#include <d3d11.h>

#include <array>

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class DirectX11Context;

    namespace detail
    {
        struct RasterizerStateHasher final
        {
            foundation::HashCode operator()( const RenderState &renderState ) const
            {
                foundation::HashCode hash = 0;
                hash ^= std::hash< int >()( static_cast< int >( renderState.CullMode ) );
                hash ^= std::hash< int >()( static_cast< int >( renderState.FillMode ) );
                hash ^= std::hash< bool >()( renderState.EnableDepthClip );
                return hash;
            }
        };

        struct RasterizerStateComparer final
        {
            bool operator()( const RenderState &lhs, const RenderState &rhs ) const
            {
                return lhs.CullMode == rhs.CullMode && lhs.FillMode == rhs.FillMode &&
                       lhs.EnableDepthClip == rhs.EnableDepthClip;
            }
        };
    }

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

        void CreateGPUBuffer( GPUBufferHandle handle, const GPUBufferDescriptor &bufferDesc ) override;
        void DestroyGPUBuffer( GPUBufferHandle handle ) override;

        memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const VertexLayout &layout,
            const std::string &techniqueName = "" ) override;
        memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const std::string &techniqueName = "" ) override;
        memory::Ref< Texture > CreateTexture2D( const std::string &filePath ) override;
        memory::Ref< Texture > CreateTextureCube( const std::string &filePath ) override;
        memory::Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor ) override;

        void InvalidateFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) override;

        DirectX11RasterizerState *GetOrCreateRasterizerState( const RenderState &renderState );

      private:
        ID3D11Device *m_pInternal = nullptr;
        ID3D11DeviceContext *m_pContext = nullptr;
        IDXGIFactory *m_pDXGIFactory = nullptr;

        std::vector< DirectX11Context * > m_pGraphicsContexts;

        std::array< DirectX11Buffer, s_MaxBufferCount > m_GPUBuffers;

        std::unordered_map< RenderState,
            Scope< DirectX11RasterizerState >,
            detail::RasterizerStateHasher,
            detail::RasterizerStateComparer >
            m_RasterizerStateCache;

        friend class DirectX11Context;
    };
}

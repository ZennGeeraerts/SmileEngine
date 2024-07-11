/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_renderer_api.h"

#include "platform/directx11/graphic/shader/directx11_shader.h"

#include "directx11_device.h"
#include "directx11_context.h"
#include "directx11_diagnostics.h"

#include "smile_engine/graphic/render_engine.h"

namespace smile::graphic
{
    DirectX11RendererAPI::~DirectX11RendererAPI()
    {
        delete s_pDevice;
        delete s_pContext;
        delete m_pSwapChain;

        SAFE_RELEASE( m_pDXGIFactory );
    }

    void DirectX11RendererAPI::Initialize( window::Window *pWindow )
    {
        // Create DXGI Factory to create SwapChain based on hardware
        HRESULT result = CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast< void ** >( &m_pDXGIFactory ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectXContext::Initialize > Failed to create DXGIFactory: %ls", GetDirectX11ErrorMessage( result ) );
            return;
        }

        auto pDirectX11Context = new DirectX11Context{};
        s_pContext = pDirectX11Context;
        s_pDevice = new DirectX11Device{ s_pContext };
        m_pSwapChain = new DirectX11SwapChain{ s_pDevice, s_pContext, pWindow, m_pDXGIFactory };
        pDirectX11Context->m_pSwapChain = m_pSwapChain;
    }

    void DirectX11RendererAPI::Draw( Uint32 vertexCount, const Ref< Shader > &pShader )
    {
        s_pContext->Draw( vertexCount, pShader );
    }

    void DirectX11RendererAPI::DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader )
    {
        s_pContext->DrawIndexed( indexCount, pShader );
    }

    void DirectX11RendererAPI::Present()
    {
        m_pSwapChain->Present();
    }

    void DirectX11RendererAPI::Clear()
    {
        s_pContext->Clear( m_ClearColor );
    }

    void DirectX11RendererAPI::ResizeWindow(Uint32 x, Uint32 y, Uint32 width, Uint32 height)
    {
        m_pSwapChain->Resize( x, y, width, height );
    }
}
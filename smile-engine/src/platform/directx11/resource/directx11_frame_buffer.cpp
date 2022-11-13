#include "smpch.h"
#include "directx11_frame_buffer.h"

#include "smile_engine/core/application.h"
#include "smile_engine/core/logger.h"

#include "platform/directx11/directx11_diagnostics.h"

namespace smile::graphic
{
    const Uint32 DirectX11Framebuffer::s_MaxFramebufferSize = 8192;

    DirectX11Framebuffer::DirectX11Framebuffer( const FramebufferDescriptor &framebufferDesc ) : m_Desc{ framebufferDesc }
    {
        m_pDirectX11Context =
            static_cast< DirectX11Context * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT( m_pDirectX11Context,
            "DirectX11Framebuffer::DirectX11Framebuffer > Rendering context is not a DirectX11RenderingContext" );

        for ( const auto &framebufferTextureData : m_Desc.Attachments.Attachments )
        {
            if ( !IsDepthFormat( framebufferTextureData.TextureFormat ) )
                m_ColorAttachmentData.emplace_back( framebufferTextureData );
            else
                m_DepthAttachmentData = framebufferTextureData;
        }

        Invalidate();
    }

    DirectX11Framebuffer::~DirectX11Framebuffer()
    {
        for ( Uint32 i{}; i < m_pRenderTargetViews.size(); ++i )
            SAFE_RELEASE( m_pRenderTargetViews[i] );

        for ( Uint32 i{}; i < m_pColorAttachments.size(); ++i )
            SAFE_RELEASE( m_pColorAttachments[i] );

        for ( Uint32 i{}; i < m_pColorShaderResourceViews.size(); ++i )
            SAFE_RELEASE( m_pColorShaderResourceViews[i] );

        m_pRenderTargetViews.clear();
        m_pColorAttachments.clear();
        m_pColorShaderResourceViews.clear();

        SAFE_RELEASE( m_pDepthStencilAttachment );
        SAFE_RELEASE( m_pDepthStencilView );
    }

    void DirectX11Framebuffer::Invalidate()
    {
        for ( Uint32 i{}; i < m_pRenderTargetViews.size(); ++i )
            SAFE_RELEASE( m_pRenderTargetViews[i] );

        for ( Uint32 i{}; i < m_pColorAttachments.size(); ++i )
            SAFE_RELEASE( m_pColorAttachments[i] );

        for ( Uint32 i{}; i < m_pColorShaderResourceViews.size(); ++i )
            SAFE_RELEASE( m_pColorShaderResourceViews[i] );

        m_pRenderTargetViews.clear();
        m_pColorAttachments.clear();
        m_pColorShaderResourceViews.clear();

        SAFE_RELEASE( m_pDepthStencilAttachment );
        SAFE_RELEASE( m_pDepthStencilView );

        // Attachments
        if ( m_ColorAttachmentData.size() )
        {
            m_pColorAttachments.resize( m_ColorAttachmentData.size() );
            m_pRenderTargetViews.resize( m_ColorAttachmentData.size() );
            m_pColorShaderResourceViews.resize( m_ColorAttachmentData.size() );

            for ( uint32_t i{}; i < m_pColorAttachments.size(); ++i )
            {
                D3D11_TEXTURE2D_DESC textureDesc = {};
                textureDesc.Width = m_Desc.Width;
                textureDesc.Height = m_Desc.Height;
                textureDesc.MipLevels = 1;
                textureDesc.ArraySize = 1;
                textureDesc.Format = FramebufferTextureFormatToDirectXBaseType( m_ColorAttachmentData[i].TextureFormat );
                textureDesc.SampleDesc.Count = m_Desc.Samples;
                textureDesc.SampleDesc.Quality = 0;
                textureDesc.Usage = D3D11_USAGE_DEFAULT;
                textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET |
                                         ( ( m_ColorAttachmentData[i].UseInShader ) ? D3D11_BIND_SHADER_RESOURCE : 0 );
                textureDesc.CPUAccessFlags = 0;
                textureDesc.MiscFlags = 0;

                HRESULT result =
                    m_pDirectX11Context->GetDevice()->CreateTexture2D( &textureDesc, nullptr, &m_pColorAttachments[i] );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create Texture2D: %ls",
                        GetDirectX11ErrorMessage( result ) );
                    return;
                }

                D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
                renderTargetViewDesc.Format = textureDesc.Format;
                renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };

                result = m_pDirectX11Context->GetDevice()->CreateRenderTargetView(
                    m_pColorAttachments[i], &renderTargetViewDesc, &m_pRenderTargetViews[i] );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create render target view: %ls",
                        GetDirectX11ErrorMessage( result ) );
                    return;
                }

                if ( m_ColorAttachmentData[i].UseInShader )
                {
                    result = m_pDirectX11Context->GetDevice()->CreateShaderResourceView(
                        m_pColorAttachments[i], nullptr, &m_pColorShaderResourceViews[i] );
                    if ( FAILED( result ) )
                    {
                        SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create shader resource view: %ls",
                            GetDirectX11ErrorMessage( result ) );
                        return;
                    }
                }
            }
        }

        if ( m_DepthAttachmentData.TextureFormat != FramebufferTextureFormat::None )
        {
            D3D11_TEXTURE2D_DESC depthStencilDesc{};
            depthStencilDesc.Width = m_Desc.Width;
            depthStencilDesc.Height = m_Desc.Height;
            depthStencilDesc.MipLevels = 1;
            depthStencilDesc.ArraySize = 1;
            depthStencilDesc.Format = FramebufferTextureFormatToDirectXBaseType( m_DepthAttachmentData.TextureFormat );
            depthStencilDesc.SampleDesc.Count = 1;
            depthStencilDesc.SampleDesc.Quality = 0;
            depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
            depthStencilDesc.BindFlags =
                D3D11_BIND_DEPTH_STENCIL | ( ( m_DepthAttachmentData.UseInShader ) ? D3D11_BIND_SHADER_RESOURCE : 0 );
            depthStencilDesc.CPUAccessFlags = 0;
            depthStencilDesc.MiscFlags = 0;

            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
            depthStencilViewDesc.Format = depthStencilDesc.Format;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0;

            HRESULT result =
                m_pDirectX11Context->GetDevice()->CreateTexture2D( &depthStencilDesc, 0, &m_pDepthStencilAttachment );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create depth stencil buffer: %ls",
                    GetDirectX11ErrorMessage( result ) );
                return;
            }

            result = m_pDirectX11Context->GetDevice()->CreateDepthStencilView(
                m_pDepthStencilAttachment, &depthStencilViewDesc, &m_pDepthStencilView );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create depth stencil view: %ls",
                    GetDirectX11ErrorMessage( result ) );
                return;
            }
        }

        /*D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = m_Data.Width;
        textureDesc.Height = m_Data.Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        HRESULT result = m_pDirectX11Context->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_pColorBuffer);
        if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create Texture2D");
            return;
        }

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };

        result = m_pDirectX11Context->GetDevice()->CreateRenderTargetView(m_pColorBuffer, &renderTargetViewDesc,
        &m_pRenderTargetView); if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create render target view");
            return;
        }

        result = m_pDirectX11Context->GetDevice()->CreateShaderResourceView(m_pColorBuffer, nullptr,
        &m_pColorShaderResourceView); if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create shader resource view");
            return;
        }

        D3D11_TEXTURE2D_DESC depthStencilDesc{};
        depthStencilDesc.Width = m_Data.Width;
        depthStencilDesc.Height = m_Data.Height;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.CPUAccessFlags = 0;
        depthStencilDesc.MiscFlags = 0;

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
        depthStencilViewDesc.Format = depthStencilDesc.Format;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

        result = m_pDirectX11Context->GetDevice()->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer);
        if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create depth stencil buffer");
            return;
        }

        result = m_pDirectX11Context->GetDevice()->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc,
        &m_pDepthStencilView); if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create depth stencil view");
            return;
        }*/

        m_Viewport.Width = static_cast< FLOAT >( m_Desc.Width );
        m_Viewport.Height = static_cast< FLOAT >( m_Desc.Height );
        m_Viewport.MinDepth = 0.0f;
        m_Viewport.MaxDepth = 1.0f;
        m_Viewport.TopLeftX = 0.0f;
        m_Viewport.TopLeftY = 0.0f;
    }

    void DirectX11Framebuffer::Bind() const
    {
        m_pDirectX11Context->GetDeviceContext()->OMSetRenderTargets(
            m_pRenderTargetViews.size(), &m_pRenderTargetViews[0], m_pDepthStencilView );
        m_pDirectX11Context->GetDeviceContext()->RSSetViewports( 1, &m_Viewport );
    }

    void DirectX11Framebuffer::Unbind() const
    {
        auto renderTargetView = m_pDirectX11Context->GetRenderTargetView();
        m_pDirectX11Context->GetDeviceContext()->OMSetRenderTargets(
            1, &renderTargetView, m_pDirectX11Context->GetDepthStencilView() );

        const auto &viewport = m_pDirectX11Context->GetViewport();
        m_pDirectX11Context->GetDeviceContext()->RSSetViewports( 1, &viewport );
    }

    void DirectX11Framebuffer::SetClearColor( const DirectX::XMFLOAT4 &color )
    {
        m_ClearColor = color;
    }

    void DirectX11Framebuffer::Clear()
    {
        const float *pClearColor = reinterpret_cast< const float * >( &m_ClearColor );
        for ( Uint32 i{}; i < m_pRenderTargetViews.size(); ++i )
            m_pDirectX11Context->GetDeviceContext()->ClearRenderTargetView( m_pRenderTargetViews[i], pClearColor );

        if ( m_DepthAttachmentData.TextureFormat != FramebufferTextureFormat::None )
            m_pDirectX11Context->GetDeviceContext()->ClearDepthStencilView(
                m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0 );
    }

    void DirectX11Framebuffer::Resize( Uint32 width, Uint32 height )
    {
        if ( ( width <= 0 ) || ( height <= 0 ) || ( width > s_MaxFramebufferSize ) || ( height > s_MaxFramebufferSize ) )
        {
            SM_LOG_WARNING( "DirectX11Framebuffer::resize > Invalid framebuffer size: %d, %d", width, height );
            return;
        }

        m_Desc.Width = width;
        m_Desc.Height = height;

        Invalidate();
    }

    DXGI_FORMAT DirectX11Framebuffer::FramebufferTextureFormatToDirectXBaseType( FramebufferTextureFormat format )
    {
        switch ( format )
        {
            case FramebufferTextureFormat::RGBA8:
                return DXGI_FORMAT_B8G8R8A8_UNORM;
                break;
            case FramebufferTextureFormat::Depth24Stencil8:
                return DXGI_FORMAT_D24_UNORM_S8_UINT;
                break;

            case FramebufferTextureFormat::None:
            default:
                SM_ASSERT(
                    false, "DirectX11Framebuffer::framebufferTextureFormatToDirectXBaseType > Unknown ShaderDataType" );
                return DXGI_FORMAT_UNKNOWN;
        }
    }

    bool DirectX11Framebuffer::IsDepthFormat( FramebufferTextureFormat format )
    {
        switch ( format )
        {
            case FramebufferTextureFormat::Depth24Stencil8:
                return true;

            default:
                return false;
        }
    }
}
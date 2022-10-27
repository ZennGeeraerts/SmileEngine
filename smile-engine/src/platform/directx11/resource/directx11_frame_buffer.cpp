#include "smpch.h"
#include "directx11_frame_buffer.h"

#include "smile_engine/core/application.h"
#include "smile_engine/core/logger.h"

#include "platform/directx11/directx11_diagnostics.h"

namespace smile::graphic
{
    const Uint32 DirectX11Framebuffer::maxFramebufferSize = 8192;

    DirectX11Framebuffer::DirectX11Framebuffer( const FramebufferData &frame_buffer_data ) : data{ frame_buffer_data }
    {
        directX11Context =
            static_cast< DirectX11Context * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT( directX11Context,
            "DirectX11Framebuffer::DirectX11Framebuffer > Rendering context is not a DirectX11RenderingContext" );

        for ( const auto &frame_buffer_texture_data : data.attachments.attachments )
        {
            if ( !isDepthFormat( frame_buffer_texture_data.textureFormat ) )
                colorAttachmentData.emplace_back( frame_buffer_texture_data );
            else
                depthAttachmentData = frame_buffer_texture_data;
        }

        invalidate();
    }

    DirectX11Framebuffer::~DirectX11Framebuffer()
    {
        for ( Uint32 i{}; i < renderTargetViews.size(); ++i )
            SAFE_RELEASE( renderTargetViews[i] );

        for ( Uint32 i{}; i < colorAttachments.size(); ++i )
            SAFE_RELEASE( colorAttachments[i] );

        for ( Uint32 i{}; i < colorShaderResourceViews.size(); ++i )
            SAFE_RELEASE( colorShaderResourceViews[i] );

        renderTargetViews.clear();
        colorAttachments.clear();
        colorShaderResourceViews.clear();

        SAFE_RELEASE( depthStencilAttachment );
        SAFE_RELEASE( depthStencilView );
    }

    void DirectX11Framebuffer::invalidate()
    {
        for ( Uint32 i{}; i < renderTargetViews.size(); ++i )
            SAFE_RELEASE( renderTargetViews[i] );

        for ( Uint32 i{}; i < colorAttachments.size(); ++i )
            SAFE_RELEASE( colorAttachments[i] );

        for ( Uint32 i{}; i < colorShaderResourceViews.size(); ++i )
            SAFE_RELEASE( colorShaderResourceViews[i] );

        renderTargetViews.clear();
        colorAttachments.clear();
        colorShaderResourceViews.clear();

        SAFE_RELEASE( depthStencilAttachment );
        SAFE_RELEASE( depthStencilView );

        // Attachments
        if ( colorAttachmentData.size() )
        {
            colorAttachments.resize( colorAttachmentData.size() );
            renderTargetViews.resize( colorAttachmentData.size() );
            colorShaderResourceViews.resize( colorAttachmentData.size() );

            for ( uint32_t i{}; i < colorAttachments.size(); ++i )
            {
                D3D11_TEXTURE2D_DESC texture_desc = {};
                texture_desc.Width = data.width;
                texture_desc.Height = data.height;
                texture_desc.MipLevels = 1;
                texture_desc.ArraySize = 1;
                texture_desc.Format = framebufferTextureFormatToDirectXBaseType( colorAttachmentData[i].textureFormat );
                texture_desc.SampleDesc.Count = data.samples;
                texture_desc.SampleDesc.Quality = 0;
                texture_desc.Usage = D3D11_USAGE_DEFAULT;
                texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET |
                                         ( ( colorAttachmentData[i].useInShader ) ? D3D11_BIND_SHADER_RESOURCE : 0 );
                texture_desc.CPUAccessFlags = 0;
                texture_desc.MiscFlags = 0;

                HRESULT result =
                    directX11Context->getDevice()->CreateTexture2D( &texture_desc, nullptr, &colorAttachments[i] );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Framebuffer::invalidate > Failed to create Texture2D: %ls",
                        getDirectX11ErrorMessage( result ) );
                    return;
                }

                D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc = {};
                render_target_view_desc.Format = texture_desc.Format;
                render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                render_target_view_desc.Texture2D = D3D11_TEX2D_RTV{ 0 };

                result = directX11Context->getDevice()->CreateRenderTargetView(
                    colorAttachments[i], &render_target_view_desc, &renderTargetViews[i] );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Framebuffer::invalidate > Failed to create render target view: %ls",
                        getDirectX11ErrorMessage( result ) );
                    return;
                }

                if ( colorAttachmentData[i].useInShader )
                {
                    result = directX11Context->getDevice()->CreateShaderResourceView(
                        colorAttachments[i], nullptr, &colorShaderResourceViews[i] );
                    if ( FAILED( result ) )
                    {
                        SM_LOG_ERROR( "DirectX11Framebuffer::invalidate > Failed to create shader resource view: %ls",
                            getDirectX11ErrorMessage( result ) );
                        return;
                    }
                }
            }
        }

        if ( depthAttachmentData.textureFormat != FramebufferTextureFormat::None )
        {
            D3D11_TEXTURE2D_DESC depth_stencil_desc{};
            depth_stencil_desc.Width = data.width;
            depth_stencil_desc.Height = data.height;
            depth_stencil_desc.MipLevels = 1;
            depth_stencil_desc.ArraySize = 1;
            depth_stencil_desc.Format = framebufferTextureFormatToDirectXBaseType( depthAttachmentData.textureFormat );
            depth_stencil_desc.SampleDesc.Count = 1;
            depth_stencil_desc.SampleDesc.Quality = 0;
            depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
            depth_stencil_desc.BindFlags =
                D3D11_BIND_DEPTH_STENCIL | ( ( depthAttachmentData.useInShader ) ? D3D11_BIND_SHADER_RESOURCE : 0 );
            depth_stencil_desc.CPUAccessFlags = 0;
            depth_stencil_desc.MiscFlags = 0;

            D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
            depth_stencil_view_desc.Format = depth_stencil_desc.Format;
            depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depth_stencil_view_desc.Texture2D.MipSlice = 0;

            HRESULT result =
                directX11Context->getDevice()->CreateTexture2D( &depth_stencil_desc, 0, &depthStencilAttachment );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Framebuffer::invalidate > Failed to create depth stencil buffer: %ls",
                    getDirectX11ErrorMessage( result ) );
                return;
            }

            result = directX11Context->getDevice()->CreateDepthStencilView(
                depthStencilAttachment, &depth_stencil_view_desc, &depthStencilView );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Framebuffer::invalidate > Failed to create depth stencil view: %ls",
                    getDirectX11ErrorMessage( result ) );
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

        viewport.Width = static_cast< FLOAT >( data.width );
        viewport.Height = static_cast< FLOAT >( data.height );
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
    }

    void DirectX11Framebuffer::bind() const
    {
        directX11Context->getDeviceContext()->OMSetRenderTargets(
            renderTargetViews.size(), &renderTargetViews[0], depthStencilView );
        directX11Context->getDeviceContext()->RSSetViewports( 1, &viewport );
    }

    void DirectX11Framebuffer::unbind() const
    {
        auto render_target_view = directX11Context->getRenderTargetView();
        directX11Context->getDeviceContext()->OMSetRenderTargets(
            1, &render_target_view, directX11Context->getDepthStencilView() );

        const auto &viewport = directX11Context->getViewport();
        directX11Context->getDeviceContext()->RSSetViewports( 1, &viewport );
    }

    void DirectX11Framebuffer::setClearColor( const DirectX::XMFLOAT4 &color )
    {
        clearColor = color;
    }

    void DirectX11Framebuffer::clear()
    {
        const float *clear_color = reinterpret_cast< const float * >( &clearColor );
        for ( Uint32 i{}; i < renderTargetViews.size(); ++i )
            directX11Context->getDeviceContext()->ClearRenderTargetView( renderTargetViews[i], clear_color );

        if ( depthAttachmentData.textureFormat != FramebufferTextureFormat::None )
            directX11Context->getDeviceContext()->ClearDepthStencilView(
                depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0 );
    }

    void DirectX11Framebuffer::resize( Uint32 width, Uint32 height )
    {
        if ( ( width <= 0 ) || ( height <= 0 ) || ( width > maxFramebufferSize ) || ( height > maxFramebufferSize ) )
        {
            SM_LOG_WARNING( "DirectX11Framebuffer::resize > Invalid framebuffer size: %d, %d", width, height );
            return;
        }

        data.width = width;
        data.height = height;

        invalidate();
    }

    DXGI_FORMAT DirectX11Framebuffer::framebufferTextureFormatToDirectXBaseType( FramebufferTextureFormat format )
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

    bool DirectX11Framebuffer::isDepthFormat( FramebufferTextureFormat format )
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
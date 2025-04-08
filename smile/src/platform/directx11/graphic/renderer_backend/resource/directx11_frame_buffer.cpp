/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_frame_buffer.h"

#include "platform/directx11/graphic/renderer_backend/directx11_diagnostics.h"

namespace smile::graphic
{
    static bool IsDepthFormat( FramebufferTextureFormat format )
    {
        switch ( format )
        {
            case FramebufferTextureFormat::Depth24Stencil8:
                return true;

            default:
                return false;
        }
    }

    static DXGI_FORMAT FramebufferTextureFormatToDirectXBaseType( FramebufferTextureFormat format )
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

    void DirectX11Framebuffer::Create( ID3D11Device *pDevice, const FramebufferDescriptor &desc )
    {
        Descriptor = desc;

        for ( const auto &framebufferTextureData : desc.Attachments.Attachments )
        {
            if ( !IsDepthFormat( framebufferTextureData.TextureFormat ) )
                ColorAttachmentData.push_back( framebufferTextureData );
            else
                DepthAttachmentData = framebufferTextureData;
        }

        Invalidate( pDevice );
    }

    void DirectX11Framebuffer::Destroy()
    {
        for ( Uint32 i{}; i < pRenderTargetViews.size(); ++i )
            SAFE_RELEASE( pRenderTargetViews[i] );

        for ( Uint32 i{}; i < pColorAttachments.size(); ++i )
            SAFE_RELEASE( pColorAttachments[i] );

        for ( Uint32 i{}; i < pColorShaderResourceViews.size(); ++i )
            SAFE_RELEASE( pColorShaderResourceViews[i] );

        pRenderTargetViews.clear();
        pColorAttachments.clear();
        pColorShaderResourceViews.clear();

        SAFE_RELEASE( pDepthStencilAttachment );
        SAFE_RELEASE( pDepthStencilView );
    }

    void DirectX11Framebuffer::Invalidate( ID3D11Device *pDevice )
    {
        for ( Uint32 i{}; i < pRenderTargetViews.size(); ++i )
            SAFE_RELEASE( pRenderTargetViews[i] );

        for ( Uint32 i{}; i < pColorAttachments.size(); ++i )
            SAFE_RELEASE( pColorAttachments[i] );

        for ( Uint32 i{}; i < pColorShaderResourceViews.size(); ++i )
            SAFE_RELEASE( pColorShaderResourceViews[i] );

        pRenderTargetViews.clear();
        pColorAttachments.clear();
        pColorShaderResourceViews.clear();

        SAFE_RELEASE( pDepthStencilAttachment );
        SAFE_RELEASE( pDepthStencilView );

        // Attachments
        if ( ColorAttachmentData.size() )
        {
            pColorAttachments.resize( ColorAttachmentData.size() );
            pRenderTargetViews.resize( ColorAttachmentData.size() );
            pColorShaderResourceViews.resize( ColorAttachmentData.size() );

            for ( uint32_t i{}; i < pColorAttachments.size(); ++i )
            {
                D3D11_TEXTURE2D_DESC textureDesc = {};
                textureDesc.Width = Descriptor.Width;
                textureDesc.Height = Descriptor.Height;
                textureDesc.MipLevels = 1;
                textureDesc.ArraySize = 1;
                textureDesc.Format = FramebufferTextureFormatToDirectXBaseType( ColorAttachmentData[i].TextureFormat );
                textureDesc.SampleDesc.Count = Descriptor.Samples;
                textureDesc.SampleDesc.Quality = 0;
                textureDesc.Usage = D3D11_USAGE_DEFAULT;
                textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET |
                                        ( ( ColorAttachmentData[i].UseInShader ) ? D3D11_BIND_SHADER_RESOURCE : 0 );
                textureDesc.CPUAccessFlags = 0;
                textureDesc.MiscFlags = 0;

                HRESULT result = pDevice->CreateTexture2D( &textureDesc, nullptr, &pColorAttachments[i] );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create Texture2D: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return;
                }

                D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
                renderTargetViewDesc.Format = textureDesc.Format;
                renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };

                result = pDevice->CreateRenderTargetView(
                    pColorAttachments[i], &renderTargetViewDesc, &pRenderTargetViews[i] );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create render target view: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return;
                }

                if ( ColorAttachmentData[i].UseInShader )
                {
                    result = pDevice->CreateShaderResourceView(
                        pColorAttachments[i], nullptr, &pColorShaderResourceViews[i] );
                    if ( FAILED( result ) )
                    {
                        SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create shader resource view: {}",
                            fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                        return;
                    }
                }
            }
        }

        if ( DepthAttachmentData.TextureFormat != FramebufferTextureFormat::None )
        {
            D3D11_TEXTURE2D_DESC depthStencilDesc{};
            depthStencilDesc.Width = Descriptor.Width;
            depthStencilDesc.Height = Descriptor.Height;
            depthStencilDesc.MipLevels = 1;
            depthStencilDesc.ArraySize = 1;
            depthStencilDesc.Format = FramebufferTextureFormatToDirectXBaseType( DepthAttachmentData.TextureFormat );
            depthStencilDesc.SampleDesc.Count = 1;
            depthStencilDesc.SampleDesc.Quality = 0;
            depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
            depthStencilDesc.BindFlags =
                D3D11_BIND_DEPTH_STENCIL | ( ( DepthAttachmentData.UseInShader ) ? D3D11_BIND_SHADER_RESOURCE : 0 );
            depthStencilDesc.CPUAccessFlags = 0;
            depthStencilDesc.MiscFlags = 0;

            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
            depthStencilViewDesc.Format = depthStencilDesc.Format;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0;

            HRESULT result = pDevice->CreateTexture2D( &depthStencilDesc, 0, &pDepthStencilAttachment );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create depth stencil buffer: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                return;
            }

            result =
                pDevice->CreateDepthStencilView( pDepthStencilAttachment, &depthStencilViewDesc, &pDepthStencilView );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Framebuffer::Invalidate > Failed to create depth stencil view: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                return;
            }
        }

        Viewport.Width = static_cast< FLOAT >( Descriptor.Width );
        Viewport.Height = static_cast< FLOAT >( Descriptor.Height );
        Viewport.MinDepth = 0.0f;
        Viewport.MaxDepth = 1.0f;
        Viewport.TopLeftX = 0.0f;
        Viewport.TopLeftY = 0.0f;
    }
}
/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_frame_buffer.h"

#include "platform/directx11/graphic/rhi/directx11_diagnostics.h"

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
                SM_ASSERT_MSG(
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
        for ( auto pRTV : pRenderTargetViews )
            pRTV.Reset();

        // TODO: Clear the render target views

        pDepthStencilView.Reset();
    }
}
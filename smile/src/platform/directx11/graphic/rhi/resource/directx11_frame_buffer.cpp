/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_frame_buffer.h"

#include "platform/directx11/graphic/rhi/directx11_device.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    void DirectX11Framebuffer::Create( DirectX11Device &device, const FramebufferDescriptor &desc )
    {
        Descriptor = desc;
        FramebufferInfo = FramebufferInfoExtented{ desc };

        for ( const auto &colorAttachment : desc.ColorAttachments )
        {
            SM_ASSERT( colorAttachment.IsValid() );
            pRenderTargetViews.PushBack( device.GetOrCreateRenderTargetViewForAttachment( colorAttachment ) );
        }

        if ( desc.DepthAttachment.IsValid() )
        {
            pDepthStencilView = device.GetOrCreateDepthStencilViewForAttachment( desc.DepthAttachment );
        }
    }

    void DirectX11Framebuffer::Destroy()
    {
        for ( auto pRTV : pRenderTargetViews )
            pRTV.Reset();

        // TODO: Clear the render target views

        pDepthStencilView.Reset();
    }
}
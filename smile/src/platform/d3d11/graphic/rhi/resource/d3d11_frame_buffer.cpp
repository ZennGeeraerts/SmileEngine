/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "d3d11_frame_buffer.h"

#include "platform/d3d11/graphic/rhi/d3d11_device.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    D3D11Framebuffer::~D3D11Framebuffer()
    {
        Destroy();
    }

    void D3D11Framebuffer::Create( D3D11Device &device, const FramebufferDescriptor &desc )
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

    void D3D11Framebuffer::Destroy()
    {
        for ( auto pRTV : pRenderTargetViews )
            pRTV.Reset();

        pRenderTargetViews.Clear();

        pDepthStencilView.Reset();
    }
}
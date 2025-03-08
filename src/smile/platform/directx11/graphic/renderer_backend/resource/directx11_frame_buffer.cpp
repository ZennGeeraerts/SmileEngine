/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_frame_buffer.h"

namespace smile::graphic
{
    DirectX11Framebuffer::~DirectX11Framebuffer()
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
}
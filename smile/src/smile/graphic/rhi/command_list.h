/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "graphics_state.h"
#include "draw_params.h"
#include "object.h"
#include "resource/texture.h"

#include "smile/common/memory/ref.h"
#include "smile/core/math/color.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic::rhi
{
    class SwapChain;

    class CommandList
    {
      public:
        virtual ~CommandList() = default;

        virtual Object GetInternal( ObjectType type ) const = 0;

        virtual void Open() = 0;
        virtual void Close() = 0;
        virtual void ClearState() = 0;

        virtual void BindBackBuffer( memory::Ref< SwapChain > pSwapChain ) const = 0;
        virtual void ClearBackBuffer( memory::Ref< SwapChain > pSwapChain,
            const math::Color &clearColor ) const = 0;

        virtual void SetGraphicsState( const GraphicsState &graphicsState ) = 0;
        virtual void Draw( const DrawParams &params ) = 0;
        virtual void DrawIndexed( const DrawIndexedParams &params ) = 0;

        virtual void FillBuffer( GPUBufferHandle handle, void *pData, Uint32 size ) const = 0;

        virtual void ClearTexture( TextureHandle handle,
            TextureSubresourceSet subresources,
            const math::Color &clearColor ) const = 0;

        virtual void ClearDepthStencilTexture( TextureHandle handle,
            TextureSubresourceSet subresources,
            bool clearDepth,
            float depth,
            bool clearStencil,
            Uint8 stencil ) const = 0;
    };
}
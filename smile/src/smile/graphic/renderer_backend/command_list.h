/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "sampler_state.h"
#include "render_handle.h"
#include "smile/common/memory/ref.h"

#include <DirectXMath.h>

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class SwapChain;

    class CommandList
    {
      public:
        virtual ~CommandList() = default;

        virtual void *GetInternal() const = 0;

        virtual void BindBackBuffer( memory::Ref< SwapChain > pSwapChain ) const = 0;
        virtual void ClearBackBuffer( memory::Ref< SwapChain > pSwapChain,
            const DirectX::XMFLOAT4 &clearColor ) const = 0;

        virtual void SetGraphicsPipeline( GraphicsPipelineHandle handle ) const = 0;

        virtual void SetVertexShaderSamplerState( const SamplerState &samplerState, Uint16 slot ) const = 0;
        virtual void SetPixelShaderSamplerState( const SamplerState &samplerState, Uint16 slot ) const = 0;

        virtual void Draw( Uint32 vertexCount ) = 0;
        virtual void DrawIndexed( Uint32 indexCount ) = 0;

        virtual void BindVertexBuffer( GPUBufferHandle handle, Uint32 stride ) const = 0;
        virtual void UnbindVertexBuffer() const = 0;

        virtual void BindIndexBuffer( GPUBufferHandle handle ) const = 0;
        virtual void UnbindIndexBuffer() const = 0;

        virtual void BindVertexShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const = 0;
        virtual void UnbindVertexShaderUniformBuffer( Uint16 slot ) const = 0;
        virtual void BindPixelShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const = 0;
        virtual void UnbindPixelShaderUniformBuffer( Uint16 slot ) const = 0;

        virtual void BindFramebuffer( FramebufferHandle handle ) const = 0;
        virtual void ClearFramebuffer( FramebufferHandle handle ) = 0;

        virtual void FillBuffer( GPUBufferHandle handle, void *pData, Uint32 size ) const = 0;
        virtual void *ReadTexture( TextureHandle handle ) const = 0;
        virtual void *ReadTexture( FramebufferHandle handle, Uint32 index ) const = 0;
    };
}
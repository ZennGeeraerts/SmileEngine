/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_state.h"
#include "smile/common/memory/ref.h"
#include "render_handle.h"

#include <DirectXMath.h>

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class SwapChain;
    class Shader;
    class Framebuffer;

    class GraphicsContext
    {
      public:
        virtual ~GraphicsContext() = default;

        virtual void *GetInternal() const = 0;

        virtual void BindBackBuffer( memory::Ref< SwapChain > pSwapChain ) const = 0;
        virtual void ClearBackBuffer( memory::Ref< SwapChain > pSwapChain,
            const DirectX::XMFLOAT4 &clearColor ) const = 0;

        virtual void SetState( const RenderState &state ) const = 0;
        virtual void Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader ) = 0;
        virtual void DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader ) = 0;

        virtual void BindVertexBuffer( GPUBufferHandle handle, Uint32 stride ) const = 0;
        virtual void UnbindVertexBuffer() const = 0;

        virtual void BindIndexBuffer( GPUBufferHandle handle ) const = 0;
        virtual void UnbindIndexBuffer() const = 0;

        virtual void BindVertexShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const = 0;
        virtual void UnbindVertexShaderUniformBuffer( Uint16 slot ) const = 0;
        virtual void BindPixelShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const = 0;
        virtual void UnbindPixelShaderUniformBuffer( Uint16 slot ) const = 0;

        virtual void BindShader( const memory::Ref< Shader > &pShader ) const = 0;
        virtual void UnbindShader() const = 0;

        virtual void BindFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) const = 0;
        virtual void ClearFramebuffer( memory::Ref< Framebuffer > pFramebuffer ) = 0;

        virtual void FillBuffer( GPUBufferHandle handle, void *pData, Uint32 size ) const = 0;
    };
}
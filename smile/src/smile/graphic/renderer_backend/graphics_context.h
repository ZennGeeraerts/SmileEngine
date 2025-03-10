/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "primitive_topology.h"
#include "memory/ref.h"

#include "resource/swap_chain.h"
#include "resource/frame_buffer.h"
#include "resource/rasterizer_state.h"
#include "shader/shader.h"

#include "render_handle.h"

#include <DirectXMath.h>

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class GraphicsContext
    {
      public:
        virtual ~GraphicsContext() = default;

        virtual void *GetInternal() const = 0;

        virtual void BindBackBuffer( memory::Ref< SwapChain > pSwapChain ) const = 0;
        virtual void ClearBackBuffer( memory::Ref< SwapChain > pSwapChain,
            const DirectX::XMFLOAT4 &clearColor ) const = 0;

        virtual void Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader ) = 0;
        virtual void DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader ) = 0;

        virtual void BindVertexBuffer( VertexBufferHandle vbHandle ) const = 0;
        virtual void UnbindVertexBuffer() const = 0;

        virtual void BindIndexBuffer( IndexBufferHandle ibHandle ) const = 0;
        virtual void UnbindIndexBuffer() const = 0;

        virtual void BindVertexShaderUniformBuffer( UniformBufferHandle ubHandle, Uint16 slot ) const = 0;
        virtual void UnbindVertexShaderUniformBuffer( Uint16 slot ) const = 0;
        virtual void BindPixelShaderUniformBuffer( UniformBufferHandle ubHandle, Uint16 slot ) const = 0;
        virtual void UnbindPixelShaderUniformBuffer( Uint16 slot ) const = 0;

        virtual void BindShader( const memory::Ref< Shader > &pShader ) const = 0;
        virtual void UnbindShader() const = 0;

        virtual void BindFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) const = 0;
        virtual void ClearFramebuffer( memory::Ref< Framebuffer > pFramebuffer ) = 0;

        virtual void BindRasterizerState( const memory::Ref< RasterizerState > &pRasterizerState ) const = 0;
        virtual void UnbindRasterizerState() const = 0;
        virtual void BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const = 0;
        virtual void UnbindPrimitiveTopology() const = 0;

        virtual void FillVertexBuffer( VertexBufferHandle vbHandle, void *pData, Uint32 vertexCount ) const = 0;
    };
}
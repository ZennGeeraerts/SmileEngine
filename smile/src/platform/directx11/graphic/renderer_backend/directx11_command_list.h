/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/command_list.h"
#include "directx11_context.h"

namespace smile::graphic
{
    class DirectX11Device;

    class DirectX11CommandList final : public CommandList
    {
      public:
        DirectX11CommandList( DirectX11Device *pDevice, std::reference_wrapper< const DirectX11Context > context );
        ~DirectX11CommandList() = default;

        DirectX11CommandList( const DirectX11CommandList & ) = delete;
        DirectX11CommandList( DirectX11CommandList && ) = delete;
        DirectX11CommandList &operator=( const DirectX11CommandList & ) = delete;
        DirectX11CommandList &operator=( DirectX11CommandList && ) = delete;

        void *GetInternal() const override
        {
            return m_Context.pImmediateContext;
        }

        void BindBackBuffer( memory::Ref< SwapChain > pSwapChain ) const override;
        void ClearBackBuffer( memory::Ref< SwapChain > pSwapChain, const DirectX::XMFLOAT4 &clearColor ) const override;

        void SetGraphicsPipeline( GraphicsPipelineHandle handle ) const override;

        void SetVertexShaderSamplerState( const SamplerState &samplerState, Uint16 slot ) const override;
        void SetPixelShaderSamplerState( const SamplerState &samplerState, Uint16 slot ) const override;

        void SetGraphicsState( const GraphicsState &graphicsState ) const override;
        void Draw( const DrawParams &params ) override;
        void DrawIndexed( const DrawIndexedParams &params ) override;

        void BindVertexBuffer( GPUBufferHandle handle, Uint32 stride ) const override;
        void UnbindVertexBuffer() const override;

        void BindIndexBuffer( GPUBufferHandle handle ) const override;
        void UnbindIndexBuffer() const override;

        void BindVertexShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const;
        void UnbindVertexShaderUniformBuffer( Uint16 slot ) const;
        void BindPixelShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const;
        void UnbindPixelShaderUniformBuffer( Uint16 slot ) const;

        void BindFramebuffer( FramebufferHandle handle ) const override;
        void ClearFramebuffer( FramebufferHandle handle ) override;

        void FillBuffer( GPUBufferHandle handle, void *pData, Uint32 size ) const override;
        void *ReadTexture( TextureHandle handle ) const override;
        void *ReadTexture( FramebufferHandle handle, Uint32 index ) const override;

      private:
        DirectX11Device *m_pDevice;
        const DirectX11Context &m_Context;
    };
}

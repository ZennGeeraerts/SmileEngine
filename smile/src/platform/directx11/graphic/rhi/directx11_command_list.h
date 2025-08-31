/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/command_list.h"
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

        void SetState( const RenderState &state ) const override;
        void SetVertexShaderSamplerState( const SamplerState &samplerState, Uint16 slot ) const override;
        void SetPixelShaderSamplerState( const SamplerState &samplerState, Uint16 slot ) const override;

        void Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader ) override;
        void DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader ) override;

        void BindVertexBuffer( GPUBufferHandle handle, Uint32 stride ) const override;
        void UnbindVertexBuffer() const override;

        void BindIndexBuffer( GPUBufferHandle handle ) const override;
        void UnbindIndexBuffer() const override;

        void BindVertexShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const;
        void UnbindVertexShaderUniformBuffer( Uint16 slot ) const;
        void BindPixelShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const;
        void UnbindPixelShaderUniformBuffer( Uint16 slot ) const;

        void BindShader( const memory::Ref< Shader > &pShader ) const override;
        void UnbindShader() const override;

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

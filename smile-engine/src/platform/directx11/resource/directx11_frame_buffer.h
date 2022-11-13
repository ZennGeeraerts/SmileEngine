#pragma once
#include "smile_engine/graphic/resource/frame_buffer.h"
#include "platform/directx11/directx11_context.h"

namespace smile::graphic
{
    class DirectX11Framebuffer final : public Framebuffer
    {
      public:
        DirectX11Framebuffer( const FramebufferDescriptor &framebufferDesc );
        virtual ~DirectX11Framebuffer();

        DirectX11Framebuffer( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer( DirectX11Framebuffer && ) = delete;
        DirectX11Framebuffer &operator=( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer &operator=( DirectX11Framebuffer && ) = delete;

        virtual void Invalidate() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void SetClearColor( const DirectX::XMFLOAT4 &color ) override;
        virtual void Clear() override;
        virtual void Resize( Uint32 width, Uint32 height ) override;

        virtual const FramebufferDescriptor &GetDescriptor() const override
        {
            return m_Desc;
        }
        virtual void *GetColor( Uint32 index ) const override
        {
            return m_pColorShaderResourceViews[index];
        }

      private:
        DXGI_FORMAT FramebufferTextureFormatToDirectXBaseType( FramebufferTextureFormat format );
        bool IsDepthFormat( FramebufferTextureFormat format );

      private:
        DirectX11Context *m_pDirectX11Context = nullptr;
        FramebufferDescriptor m_Desc;

        DirectX::XMFLOAT4 m_ClearColor = { 1.f, 1.f, 1.f, 1.f };

        std::vector< FramebufferTextureData > m_ColorAttachmentData;
        FramebufferTextureData m_DepthAttachmentData = FramebufferTextureFormat::None;

        std::vector< ID3D11Texture2D * > m_pColorAttachments;
        std::vector< ID3D11RenderTargetView * > m_pRenderTargetViews;
        std::vector< ID3D11ShaderResourceView * > m_pColorShaderResourceViews;

        ID3D11Texture2D *m_pDepthStencilAttachment = nullptr;
        ID3D11DepthStencilView *m_pDepthStencilView = nullptr;

        D3D11_VIEWPORT m_Viewport;

        static const Uint32 s_MaxFramebufferSize;
    };
}

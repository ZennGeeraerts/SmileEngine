#pragma once
#include "smile_engine/renderer/resource/frame_buffer.h"
#include "platform/directx11/directx11_context.h"

namespace smile::renderer
{
    class DirectX11Framebuffer final : public Framebuffer
    {
      public:
        DirectX11Framebuffer( const FramebufferData &frame_buffer_data );
        virtual ~DirectX11Framebuffer();

        DirectX11Framebuffer( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer( DirectX11Framebuffer && ) = delete;
        DirectX11Framebuffer &operator=( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer &operator=( DirectX11Framebuffer && ) = delete;

        virtual void invalidate() override;

        virtual void bind() const override;
        virtual void unbind() const override;
        virtual void setClearColor( const DirectX::XMFLOAT4 &color ) override;
        virtual void clear() override;
        virtual void resize( Uint32 width, Uint32 height ) override;

        virtual const FramebufferData &getData() const override
        {
            return data;
        }
        virtual void *getColor( Uint32 index ) const override
        {
            return colorShaderResourceViews[index];
        }

      private:
        DXGI_FORMAT framebufferTextureFormatToDirectXBaseType( FramebufferTextureFormat format );
        bool isDepthFormat( FramebufferTextureFormat format );

      private:
        DirectX11Context *directX11Context = nullptr;
        FramebufferData data;

        DirectX::XMFLOAT4 clearColor = { 1.f, 1.f, 1.f, 1.f };

        std::vector< FramebufferTextureData > colorAttachmentData;
        FramebufferTextureData depthAttachmentData = FramebufferTextureFormat::None;

        std::vector< ID3D11Texture2D * > colorAttachments;
        std::vector< ID3D11RenderTargetView * > renderTargetViews;
        std::vector< ID3D11ShaderResourceView * > colorShaderResourceViews;

        ID3D11Texture2D *depthStencilAttachment = nullptr;
        ID3D11DepthStencilView *depthStencilView = nullptr;

        D3D11_VIEWPORT viewport;

        static const Uint32 maxFramebufferSize;
    };
}

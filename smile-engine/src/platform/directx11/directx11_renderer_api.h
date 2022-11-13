#pragma once

#include "smile_engine/graphic/renderer_api.h"
#include "directx11_context.h"

namespace smile::graphic
{
    class DirectX11RendererAPI final : public RendererAPI
    {
      public:
        DirectX11RendererAPI() = default;
        DirectX11RendererAPI( const DirectX11RendererAPI & ) = delete;
        DirectX11RendererAPI( DirectX11RendererAPI && ) = delete;

        virtual void Initialize() override;
        virtual void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;
        virtual void SetClearColor( const DirectX::XMFLOAT4 &color ) override;
        virtual void Clear() override;

        virtual void DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader ) override;

      private:
        DirectX::XMFLOAT4 m_ClearColor = { 1.f, 1.f, 1.f, 1.f };
        DirectX11Context *m_pDirectX11Context = nullptr;
    };
}

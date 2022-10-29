#pragma once

#include "smile_engine/graphic/renderer_api.h"
#include "smileraster_context.h"

namespace Smile::Graphic
{
    class SmileRasterRendererAPI final : public RendererAPI
    {
      public:
        virtual void Initialize() override;
        virtual void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;
        virtual void SetClearColor( const DirectX::XMFLOAT4 &color ) override;
        virtual void Clear() override;

        virtual void DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader ) override;

      private:
        DirectX::XMFLOAT4 m_ClearColor = { 1.f, 1.f, 1.f, 1.f };
        Window *m_pWindow = nullptr;
        SmileRasterContext *m_pSmileRasterContext = nullptr;
    };
}

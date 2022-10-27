#pragma once

#include "smile_engine/graphic/renderer_api.h"
#include "smileraster_context.h"

namespace smile::graphic
{
    class SmileRasterRendererAPI final : public RendererAPI
    {
      public:
        virtual void initialize() override;
        virtual void resizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;
        virtual void setClearColor( const DirectX::XMFLOAT4 &color ) override;
        virtual void clear() override;

        virtual void drawIndexed( Uint32 index_count, const Ref< Shader > &shader ) override;

      private:
        DirectX::XMFLOAT4 clearColor = { 1.f, 1.f, 1.f, 1.f };
        Window *window = nullptr;
        SmileRasterContext *smileRasterContext = nullptr;
    };
}

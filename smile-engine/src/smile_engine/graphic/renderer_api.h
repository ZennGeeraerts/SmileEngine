#pragma once

#include "graphics_context.h"
#include "shader/shader.h"

namespace smile::graphic
{
    class RendererAPI
    {
      public:
        enum class API
        {
            None = 0,
            DirectX11 = 1,
            SmileRaster = 2
        };

      public:
        virtual ~RendererAPI() = default;

        virtual void initialize() = 0;
        virtual void resizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) = 0;
        virtual void setClearColor( const DirectX::XMFLOAT4 &color ) = 0;
        virtual void clear() = 0;

        virtual void drawIndexed( Uint32 index_count, const Ref< Shader > &shader ) = 0;

        inline static API getAPI()
        {
            return api;
        }

      private:
        static API api;
    };
}

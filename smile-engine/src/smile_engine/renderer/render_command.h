#pragma once

#include "smile_engine/core/core.h"
#include "renderer_api.h"

namespace smile::renderer
{
    class RenderCommand final
    {
      public:
        inline static void initalize()
        {
            rendererAPI->initialize();
        }

        inline static void shutDown()
        {
            SAFE_DELETE( rendererAPI );
        }

        inline static void resizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
        {
            rendererAPI->resizeWindow( x, y, width, height );
        }

        inline static void setClearColor( const DirectX::XMFLOAT4 &color )
        {
            rendererAPI->setClearColor( color );
        }

        inline static void clear()
        {
            rendererAPI->clear();
        }

        inline static void drawIndexed( int32_t index_count, const Ref< Shader > &shader )
        {
            rendererAPI->drawIndexed( index_count, shader );
        }

      private:
        static RendererAPI *rendererAPI;
    };
}

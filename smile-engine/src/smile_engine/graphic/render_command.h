#pragma once

#include "smile_engine/core/core.h"
#include "renderer_api.h"

namespace smile::graphic
{
    class RenderCommand final
    {
      public:
        inline static void Initalize()
        {
            s_RendererAPI->Initialize();
        }

        inline static void ShutDown()
        {
            SAFE_DELETE( s_RendererAPI );
        }

        inline static void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
        {
            s_RendererAPI->ResizeWindow( x, y, width, height );
        }

        inline static void SetClearColor( const DirectX::XMFLOAT4 &color )
        {
            s_RendererAPI->SetClearColor( color );
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }

        inline static void DrawIndexed( int32_t indexCount, const Ref< Shader > &pShader )
        {
            s_RendererAPI->DrawIndexed( indexCount, pShader );
        }

      private:
        static RendererAPI *s_RendererAPI;
    };
}

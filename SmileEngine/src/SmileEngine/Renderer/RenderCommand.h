#pragma once

#include "SmileEngine/Core/Core.h"
#include "RendererAPI.h"

namespace smile
{
    class RenderCommand final
    {
      public:
        inline static void Initalize()
        {
            s_pRendererAPI->Initialize();
        }

        inline static void ShutDown()
        {
            SAFE_DELETE( s_pRendererAPI );
        }

        inline static void ResizeWindow( uint32_t x, uint32_t y, uint32_t width, uint32_t height )
        {
            s_pRendererAPI->ResizeWindow( x, y, width, height );
        }

        inline static void SetClearColor( const DirectX::XMFLOAT4 &color )
        {
            s_pRendererAPI->SetClearColor( color );
        }

        inline static void Clear()
        {
            s_pRendererAPI->Clear();
        }

        inline static void DrawIndexed( int32_t indexCount, const Ref< Shader > &pShader )
        {
            s_pRendererAPI->DrawIndexed( indexCount, pShader );
        }

      private:
        static RendererAPI *s_pRendererAPI;
    };
}

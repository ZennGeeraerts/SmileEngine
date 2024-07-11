/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/common/compiled/compiled.h"
#include "renderer_api.h"

namespace smile::graphic
{
    class RenderCommand final
    {
      public:
        static void Create( RendererAPI::API api );

        inline static void Initalize( window::Window *pWindow )
        {
            s_pRendererAPI->Initialize( pWindow );
        }

        inline static void ShutDown()
        {
            SAFE_DELETE( s_pRendererAPI );
        }

        inline static void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
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

        inline static void DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader )
        {
            s_pRendererAPI->DrawIndexed( indexCount, pShader );
        }

        inline static void Draw( Uint32 vertexCount, const Ref< Shader > &pShader )
        {
            s_pRendererAPI->Draw( vertexCount, pShader );
        }

        inline static void Present()
        {
            s_pRendererAPI->Present();
        }

      private:
        static RendererAPI *s_pRendererAPI;
    };
}

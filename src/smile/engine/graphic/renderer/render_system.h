/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/compiled.h"
#include "engine/graphic/renderer_api/renderer_api.h"

namespace smile::graphic
{
    class RenderSystem final
    {
      public:
        inline static void Initalize( window::Window *pWindow )
        {
            s_pRendererAPI->Initialize( pWindow );
        }

        inline static void ShutDown()
        {
        }

        inline static void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
        {
            s_pRendererAPI->GetSwapChain()->Resize( x, y, width, height );
        }

        inline static void SetClearColor( const DirectX::XMFLOAT4 &color )
        {
            s_ClearColor = color;
        }

        inline static void Clear()
        {
            s_pRendererAPI->GetGraphicsContext()->Clear( s_ClearColor );
        }

        inline static void DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader )
        {
            s_pRendererAPI->GetGraphicsContext()->DrawIndexed( indexCount, pShader );
        }

        inline static void Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader )
        {
            s_pRendererAPI->GetGraphicsContext()->Draw( vertexCount, pShader );
        }

        inline static void Present()
        {
            s_pRendererAPI->GetSwapChain()->Present();
        }

        inline static GraphicsDevice *GetGraphicsDevice()
        {
            return s_pRendererAPI->GetGraphicsDevice();
        }

        inline static GraphicsContext *GetGraphicsContext()
        {
            return s_pRendererAPI->GetGraphicsContext();
        }

      private:
        static Scope< RendererAPI > s_pRendererAPI;
        static DirectX::XMFLOAT4 s_ClearColor;
    };
}

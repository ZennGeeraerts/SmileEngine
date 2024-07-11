/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "graphics_device.h"
#include "graphics_context.h"

namespace window
{
    class Window;
}

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

        virtual void Initialize( window::Window *pWindow ) = 0;

        virtual void Draw( Uint32 vertexCount, const Ref< Shader > &pShader ) = 0;
        virtual void DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader ) = 0;
        virtual void Present() = 0;
        virtual void Clear() = 0;

        virtual void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) = 0;

        inline static API GetAPI()
        {
            return s_API;
        }

        inline static GraphicsDevice *GetGraphicsDevice()
        {
            return s_pDevice;
        }

        inline static GraphicsContext *GetGraphicsContext()
        {
            return s_pContext;
        }

        inline void SetClearColor( const DirectX::XMFLOAT4 &color )
        {
            m_ClearColor = color;
        }

      protected:
        static GraphicsDevice *s_pDevice;
        static GraphicsContext *s_pContext;

        DirectX::XMFLOAT4 m_ClearColor{};

      private:
        static API s_API;
    };
}

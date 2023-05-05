/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
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

        virtual void Initialize() = 0;
        virtual void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) = 0;
        virtual void SetClearColor( const DirectX::XMFLOAT4 &color ) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader ) = 0;

        inline static API GetAPI()
        {
            return s_API;
        }

      private:
        static API s_API;
    };
}

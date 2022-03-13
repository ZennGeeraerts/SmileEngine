#pragma once

#include "graphics_context.h"
#include "shader/shader.h"

namespace smile
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
        virtual void ResizeWindow( uint32_t x, uint32_t y, uint32_t width, uint32_t height ) = 0;
        virtual void SetClearColor( const DirectX::XMFLOAT4 &color ) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed( int32_t indexCount, const Ref< Shader > &pShader ) = 0;

        inline static API GetAPI()
        {
            return s_API;
        }

      private:
        static API s_API;
    };
}

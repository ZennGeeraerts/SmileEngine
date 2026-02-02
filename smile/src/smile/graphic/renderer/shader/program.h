#pragma once

#include "vertex_shader.h"
#include "pixel_shader.h"

namespace smile::graphic
{
    class Program final
    {
      public:
        struct ReflectionData final
        {
            rhi::BufferLayout VertexLayout;
        };

        static Program Create( VertexShader::ConstRef vertexShader, PixelShader::ConstRef pixelShader );

      private:
        Program( VertexShader::ConstRef vertexShader, PixelShader::ConstRef pixelShader )
            : m_VertexShader{ vertexShader }, m_PixelShader{ pixelShader }
        {
        }

        VertexShader::ConstRef m_VertexShader;
        PixelShader::ConstRef m_PixelShader;
    };
}
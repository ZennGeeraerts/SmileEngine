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
            primitive::HashMap< primitive::String, ConstantBufferDescriptor > ConstantBufferDescs;
            primitive::HashMap< primitive::String, rhi::BindingLayoutElement > ShaderResourceBindings;
        };

        static Program Create( VertexShader::ConstRef vertexShader, PixelShader::ConstRef pixelShader );

      private:
        Program( VertexShader::ConstRef vertexShader,
            PixelShader::ConstRef pixelShader,
            const ReflectionData &reflectionData )
            : m_VertexShader{ vertexShader }, m_PixelShader{ pixelShader }, m_ReflectionData{ reflectionData }
        {
        }

        VertexShader::ConstRef m_VertexShader;
        PixelShader::ConstRef m_PixelShader;
        ReflectionData m_ReflectionData;
    };
}
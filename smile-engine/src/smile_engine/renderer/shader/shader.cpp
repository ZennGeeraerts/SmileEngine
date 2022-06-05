#include "smpch.h"
#include "shader.h"

#include "smile_engine/renderer/renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "platform/directx11/shader/directx11_shader.h"
#    include "platform/smileraster/smileraster_shader.h"
#endif

namespace smile
{
    Ref< Shader > Shader::Create( const std::string &filePath, const BufferLayout &bufferLayout )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Shader::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return CreateRef< DirectX11Shader >( filePath, bufferLayout );

            case RendererAPI::API::SmileRaster:
                return CreateRef< SmileRasterShader >( filePath, bufferLayout );
#endif
        }

        SM_ASSERT( false,
            "Shader::Create > return nullptr, unknown render api or render api is not supported on this platform" );
        return nullptr;
    }

    Ref< Shader > Shader::Create( const std::string &filePath )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Shader::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return CreateRef< DirectX11Shader >( filePath );
#endif
        }

        SM_ASSERT( false,
            "Shader::Create > return nullptr, unknown render api or render api is not supported on this platform" );
        return nullptr;
    }
}
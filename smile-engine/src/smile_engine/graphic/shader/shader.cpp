#include "smpch.h"
#include "shader.h"

#include "smile_engine/graphic/renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "platform/directx11/shader/directx11_shader.h"
#    include "platform/smileraster/smileraster_shader.h"
#endif

namespace smile::graphic
{
    Ref< Shader > Shader::create( const std::string &file_path, const BufferLayout &buffer_layout )
    {
        switch ( Renderer::getAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Shader::create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return createRef< DirectX11Shader >( file_path, buffer_layout );

            case RendererAPI::API::SmileRaster:
                return createRef< SmileRasterShader >( file_path, buffer_layout );
#endif
        }

        SM_ASSERT( false,
            "Shader::create > return nullptr, unknown render api or render api is not supported on this platform" );
        return nullptr;
    }

    Ref< Shader > Shader::create( const std::string &file_path )
    {
        switch ( Renderer::getAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Shader::create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return createRef< DirectX11Shader >( file_path );
#endif
        }

        SM_ASSERT( false,
            "Shader::create > return nullptr, unknown render api or render api is not supported on this platform" );
        return nullptr;
    }
}
#include "smpch.h"
#include "texture.h"

#include "smile_engine/graphic/renderer.h"
#include "platform/directx11/resource/directx11_texture.h"
#include "platform/smileraster/smileraster_texture.h"

namespace smile::graphic
{
    Ref< Texture2D > Texture2D::create( const std::string &file_path )
    {
        switch ( Renderer::getAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Shader::create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return createRef< DirectX11Texture2D >( file_path );

            case RendererAPI::API::SmileRaster:
                return createRef< DirectX11Texture2D >( file_path );
#endif
        }

        SM_ASSERT( false,
            "Shader::create > return nullptr, unknown render api or render api is not supported on this platform" );
        return nullptr;
    }
}
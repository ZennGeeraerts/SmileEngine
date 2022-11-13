#include "smpch.h"
#include "texture.h"

#include "smile_engine/graphic/renderer.h"
#include "platform/directx11/resource/directx11_texture.h"
#include "platform/smileraster/smileraster_texture.h"

namespace smile::graphic
{
    Ref< Texture2D > Texture2D::Create( const std::string &filePath )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Shader::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return CreateRef< DirectX11Texture2D >( filePath );

            case RendererAPI::API::SmileRaster:
                return CreateRef< DirectX11Texture2D >( filePath );
#endif
        }

        SM_ASSERT( false,
            "Shader::Create > return nullptr, unknown render api or render api is not supported on this platform" );
        return nullptr;
    }
}
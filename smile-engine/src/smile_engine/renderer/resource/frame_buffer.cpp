#include "smpch.h"
#include "frame_buffer.h"

#include "smile_engine/renderer/renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "platform/directx11/resource/directx11_frame_buffer.h"
#endif

namespace smile::renderer
{
    Ref< Framebuffer > Framebuffer::create( const FramebufferData &frame_buffer_data )
    {
        switch ( Renderer::getAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Framebuffer::create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return createRef< DirectX11Framebuffer >( frame_buffer_data );

            case RendererAPI::API::SmileRaster:
                return nullptr;
#endif
        }

        SM_ASSERT( false,
            "Framebuffer::create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}
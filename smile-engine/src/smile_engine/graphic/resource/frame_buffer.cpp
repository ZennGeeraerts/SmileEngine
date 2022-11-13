#include "smpch.h"
#include "frame_buffer.h"

#include "smile_engine/graphic/renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "platform/directx11/resource/directx11_frame_buffer.h"
#endif

namespace smile::graphic
{
    Ref< Framebuffer > Framebuffer::Create( const FramebufferDescriptor &framebufferDesc )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Framebuffer::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return CreateRef< DirectX11Framebuffer >( framebufferDesc );

            case RendererAPI::API::SmileRaster:
                return nullptr;
#endif
        }

        SM_ASSERT( false,
            "Framebuffer::Create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}
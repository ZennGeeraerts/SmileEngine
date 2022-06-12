#include "smpch.h"
#include "rasterizer_state.h"

#include "smile_engine/renderer/renderer.h"

#include "platform/directx11/resource/directx11_rasterizer_state.h"

namespace smile::renderer
{
    Ref< RasterizerState > RasterizerState::create( const RasterizerStateData &rasterizer_state_data )
    {
        switch ( Renderer::getAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "RasterizerState::create > return nullptr, no renderer api selected" );
                return nullptr;

            case RendererAPI::API::DirectX11:
                return createRef< DirectX11RasterizerState >( rasterizer_state_data );

            case RendererAPI::API::SmileRaster:
                return nullptr;
        }

        SM_ASSERT( false,
            "RasterizerState::create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}
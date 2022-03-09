#include "smpch.h"
#include "RasterizerState.h"

#include "SmileEngine/Renderer/Renderer.h"

#include "Platform/DirectX11/Resource/DirectX11RasterizerState.h"

namespace smile
{
    Ref< RasterizerState > RasterizerState::Create( const RasterizerStateData &rasterizerStateData )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "RasterizerState::Create > return nullptr, no renderer api selected" );
                return nullptr;

            case RendererAPI::API::DirectX11:
                return CreateRef< DirectX11RasterizerState >( rasterizerStateData );

            case RendererAPI::API::SmileRaster:
                return nullptr;
        }

        SM_ASSERT( false,
            "RasterizerState::Create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}
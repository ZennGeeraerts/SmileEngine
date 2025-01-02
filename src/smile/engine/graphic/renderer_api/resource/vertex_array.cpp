/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "vertex_array.h"

#include "smile/engine/graphic/renderer_api/renderer_api.h"

namespace smile::graphic
{
    VertexBuffer *VertexArray::Create()
    {
        switch ( RendererAPI::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "VertexArray::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return nullptr;

            case RendererAPI::API::SmileRaster:
                return nullptr;
#endif
        }

        SM_ASSERT( false,
            "VertexArray::Create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}
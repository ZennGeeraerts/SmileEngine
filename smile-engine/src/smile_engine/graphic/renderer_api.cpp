/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer_api.h"

namespace smile::graphic
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::DirectX11;
    GraphicsDevice *RendererAPI::s_pDevice = nullptr;
    GraphicsContext *RendererAPI::s_pContext = nullptr;
}
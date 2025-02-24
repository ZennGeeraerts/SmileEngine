/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_system.h"

namespace smile::graphic
{
    Scope< RendererAPI > RenderSystem::s_pRendererAPI = RendererAPI::Create( RendererAPI::API::DirectX11 );
    DirectX::XMFLOAT4 RenderSystem::s_ClearColor{};
}
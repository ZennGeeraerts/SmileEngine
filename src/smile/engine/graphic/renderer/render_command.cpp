/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_command.h"

namespace smile::graphic
{
    Scope< RendererAPI > RenderCommand::s_pRendererAPI = RendererAPI::Create( RendererAPI::API::DirectX11 );
    DirectX::XMFLOAT4 RenderCommand::s_ClearColor{};
}
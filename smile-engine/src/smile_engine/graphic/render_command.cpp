/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_command.h"

#include "platform/directx11/graphic/directx11_renderer_api.h"

#if WITH_SMILERASTER
    #include "platform/smileraster/graphic/smileraster_renderer_api.h"
#endif

namespace smile::graphic
{
    RendererAPI *RenderCommand::s_pRendererAPI = new DirectX11RendererAPI{};
}
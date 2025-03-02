/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_renderer_api.h"

#include "directx11_device.h"
#include "directx11_context.h"

namespace smile::graphic
{
    DirectX11RendererAPI::DirectX11RendererAPI() : RendererAPI{ API::DirectX11 }
    {
        auto pDirectX11Context = new DirectX11Context{};
        m_pDevice = new DirectX11Device{ pDirectX11Context };
        m_pContext = pDirectX11Context;
    }

    DirectX11RendererAPI::~DirectX11RendererAPI()
    {
        delete m_pDevice;
        delete m_pContext;
    }
}
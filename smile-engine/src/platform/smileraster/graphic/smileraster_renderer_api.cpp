/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smileraster_renderer_api.h"

#include "smileraster_context.h"
#include "smile_raster_device.h"
#include "smile_engine/core/window/window.h"

namespace smile::graphic
{
    void SmileRasterRendererAPI::Initialize( window::Window *pWindow )
    {
        m_pWindow = pWindow;

        auto pSmileRasterContext = new SmileRasterContext{};
        m_pContext = pSmileRasterContext;
        auto pSmileRasterDevice = new SmileRasterDevice{ pSmileRasterContext->m_pDeviceContext };
        m_pDevice = pSmileRasterDevice;
        auto pSmileRasterSwapChain = new SmileRasterSwapChain{ pSmileRasterContext->m_pDeviceContext, pWindow };
        m_pSwapChain = pSmileRasterSwapChain;
    }
}
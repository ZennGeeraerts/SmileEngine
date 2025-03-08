/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer_backend.h"

namespace smile::graphic
{
    RendererBackend::RendererBackend( RendererBackendType backendType ) : m_BackendType{ backendType }
    {
        m_pDevice = GraphicsDevice::Create( backendType );
        m_pContext = m_pDevice->CreateGraphicsContext();
    }

    RendererBackend::~RendererBackend()
    {
        delete m_pDevice;
    }
}
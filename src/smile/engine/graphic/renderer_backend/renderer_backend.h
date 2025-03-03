/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "graphics_device.h"

namespace smile::graphic
{
    class RendererBackend final
    {
      public:
        RendererBackend( RendererBackendType backendType );
        ~RendererBackend();

        inline GraphicsDevice *GetGraphicsDevice() const
        {
            return m_pDevice;
        }

        inline GraphicsContext *GetGraphicsContext() const
        {
            return m_pContext;
        }

        inline RendererBackendType GetType() const
        {
            return m_BackendType;
        }

      private:
        GraphicsDevice *m_pDevice = nullptr;
        GraphicsContext *m_pContext = nullptr;

        RendererBackendType m_BackendType;
    };
}

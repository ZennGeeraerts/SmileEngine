/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "graphics_device.h"
#include "graphics_context.h"

namespace smile::graphic
{
    class RendererAPI
    {
      public:
        enum class API
        {
            None = 0,
            DirectX11 = 1
        };

      public:
        RendererAPI( API api ) : m_API{ api }
        {
        }
        virtual ~RendererAPI() = default;

        inline API GetAPI()
        {
            return m_API;
        }

        inline GraphicsDevice *GetGraphicsDevice() const
        {
            return m_pDevice;
        }

        inline GraphicsContext *GetGraphicsContext() const
        {
            return m_pContext;
        }

        static Scope< RendererAPI > Create( API api );

      protected:
        GraphicsDevice *m_pDevice;
        GraphicsContext *m_pContext;

        API m_API;
    };
}

/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "graphics_device.h"
#include "graphics_context.h"
#include "swap_chain.h"

namespace window
{
    class Window;
}

namespace smile::graphic
{
    class RendererAPI
    {
      public:
        enum class API
        {
            None = 0,
            DirectX11 = 1,
            SmileRaster = 2
        };

      public:
        RendererAPI( API api ) : m_API{ api }
        {
        }
        virtual ~RendererAPI() = default;

        virtual void Initialize( window::Window *pWindow ) = 0;

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

        inline SwapChain *GetSwapChain() const
        {
            return m_pSwapChain;
        }

        static Scope< RendererAPI > Create( API api );

      protected:
        GraphicsDevice *m_pDevice;
        GraphicsContext *m_pContext;
        SwapChain *m_pSwapChain;

        API m_API;
    };
}

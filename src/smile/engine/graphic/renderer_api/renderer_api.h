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
        virtual ~RendererAPI() = default;

        virtual void Initialize( window::Window *pWindow ) = 0;

        inline static API GetAPI()
        {
            return s_API;
        }

        inline GraphicsDevice *GetGraphicsDevice() const
        {
            return m_pDevice;
        }

        inline GraphicsContext *GetGraphicsContext() const
        {
            return m_pContext;
        }

        inline SwapChain* GetSwapChain() const
        {
            return m_pSwapChain;
        }

      protected:
        GraphicsDevice *m_pDevice;
        GraphicsContext *m_pContext;
        SwapChain *m_pSwapChain;

      private:
        static API s_API;
    };
}

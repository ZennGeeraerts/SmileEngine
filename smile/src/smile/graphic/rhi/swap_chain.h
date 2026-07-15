/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "object.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic::rhi
{
    class SwapChain
    {
      public:
        SwapChain( const window::Window *pWindow ) : m_pWindow{ pWindow }
        {
        }

        virtual ~SwapChain() = default;

        virtual void Present() const = 0;
        virtual void Resize( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) = 0;

        virtual Object GetNativeRenderTarget() const = 0;

      protected:
        const window::Window *m_pWindow;
    };
}
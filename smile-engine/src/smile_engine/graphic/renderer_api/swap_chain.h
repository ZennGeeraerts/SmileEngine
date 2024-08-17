/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class SwapChain
    {
      public:
        SwapChain( window::Window *pWindow )
            : m_pWindow{ pWindow }
        {
        }

        virtual ~SwapChain() = default;

        virtual void Present() = 0;
        virtual void Resize( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) = 0;

      protected:
        window::Window *m_pWindow;
    };
}
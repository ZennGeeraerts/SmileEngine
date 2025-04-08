/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class SwapChain : public memory::Counted
    {
      public:
        SwapChain( const window::Window *pWindow ) : m_pWindow{ pWindow }
        {
        }

        virtual ~SwapChain() = default;

        virtual void Present() = 0;

      protected:
        const window::Window *m_pWindow;
    };
}
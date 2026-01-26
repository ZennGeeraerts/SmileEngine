/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/vector.h"
#include "window.h"

namespace smile::window
{
    class WindowManager
    {
      public:
        WindowManager() = default;
        virtual ~WindowManager() = default;

        virtual Window *CreateNewWindow( const WindowSettings &windowSettings ) = 0;
        virtual void PollEvents() = 0;

        Window::Ref GetWindow( const Index index ) const
        {
            SM_ASSERT( index < m_pWindows.GetItemCount() );

            return m_pWindows[index];
        }

        static WindowManager *Create();

      protected:
        primitive::Vector< Window::Ref > m_pWindows{};
    };
}
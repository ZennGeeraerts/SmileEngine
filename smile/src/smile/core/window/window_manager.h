/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/common/foundation/singleton.h"
#include "window.h"

namespace smile::window
{
    class WindowManager
    {
      public:
        virtual ~WindowManager()
        {
            for ( auto pWindow : m_pWindows )
                delete pWindow;
        }

        virtual Window *CreateNewWindow( const WindowSettings &windowSettings ) = 0;
        virtual void PollEvents() = 0;

        Window *GetWindow( Uint32 index )
        {
            SM_ASSERT( index < m_pWindows.size() && index >= 0, "" );
            return m_pWindows[index];
        }

        const Window *GetWindow( Uint32 index ) const
        {
            SM_ASSERT( index < m_pWindows.size() && index >= 0, "" );
            return m_pWindows[index];
        }

        static WindowManager *Create();

      protected:
        std::vector< Window * > m_pWindows{};
    };
}
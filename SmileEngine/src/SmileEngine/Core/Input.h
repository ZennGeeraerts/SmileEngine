#pragma once

#include "SmileEngine/Core/Core.h"

namespace smile
{
    class Input
    {
      public:
        inline static Input *GetInstance()
        {
            return s_pInstance;
        }

        inline static bool IsKeyPressed( int keycode )
        {
            return s_pInstance->IsKeyPressedImpl( keycode );
        }

        inline static bool IsMouseButtonPressed( int button )
        {
            return s_pInstance->IsMouseButtonPressedImpl( button );
        }

        inline static std::pair< float, float > GetMousePosition()
        {
            return s_pInstance->GetMousePositionImpl();
        }

        inline static float GetMouseX()
        {
            return s_pInstance->GetMouseXImpl();
        }

        inline static float GetMouseY()
        {
            return s_pInstance->GetMouseYImpl();
        }

      protected:
        virtual bool IsKeyPressedImpl( int keycode ) = 0;
        virtual bool IsMouseButtonPressedImpl( int button ) = 0;
        virtual std::pair< float, float > GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

      private:
        static Input *s_pInstance;
    };
}

#pragma once

#include "SmileEngine/Core/Core.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace smile
{
    class Input
    {
      public:
        static bool IsKeyPressed( KeyCode keyCode );

        static bool IsMouseButtonPressed( MouseCode mouseCode );
        static DirectX::XMFLOAT2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}

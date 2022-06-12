#pragma once

#include "smile_engine/core/core.h"

#include "key_codes.h"
#include "mouse_codes.h"

namespace smile::input
{
    class Input
    {
     public:
        static bool isKeyPressed( KeyCode key_code );

        static bool isMouseButtonPressed( MouseCode mouse_code );
        static DirectX::XMFLOAT2 getMousePosition();
        static float getMouseX();
        static float getMouseY();
    };
}

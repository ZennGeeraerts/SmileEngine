/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/common/foundation/compiled.h"

#include "key_codes.h"
#include "mouse_codes.h"

#include <DirectXMath.h>

namespace smile::input
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

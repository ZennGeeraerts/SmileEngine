#include "smpch.h"
#include "smile_engine/input/input.h"
#include "smile_engine/core/application.h"

namespace smile::input
{
    bool Input::IsKeyPressed( KeyCode keyCode )
    {
        auto state = GetKeyState( keyCode );
        return state & 0x8000;
    }

    bool Input::IsMouseButtonPressed( MouseCode mouseCode )
    {
        return IsKeyPressed( mouseCode );
    }

    DirectX::XMFLOAT2 Input::GetMousePosition()
    {
        POINT point{};
        if ( GetCursorPos( &point ) )
        {
            auto window = static_cast< HWND >( Application::GetInstance().GetWindow().GetNativeWindow() );
            ScreenToClient( window, &point );
        }
        return DirectX::XMFLOAT2{ static_cast< float >( point.x ), static_cast< float >( point.y ) };
    }

    float Input::GetMouseX()
    {
        auto mousePosition = GetMousePosition();
        return mousePosition.x;
    }

    float Input::GetMouseY()
    {
        auto mousePosition = GetMousePosition();
        return mousePosition.y;
    }
}
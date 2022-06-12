#include "smpch.h"
#include "smile_engine/input/input.h"
#include "smile_engine/core/application.h"

namespace smile::input
{
    bool Input::isKeyPressed( KeyCode key_code )
    {
        auto state = GetKeyState( key_code );
        return state & 0x8000;
    }

    bool Input::isMouseButtonPressed( MouseCode mouse_code )
    {
        return isKeyPressed( mouse_code );
    }

    DirectX::XMFLOAT2 Input::getMousePosition()
    {
        POINT point{};
        if ( GetCursorPos( &point ) )
        {
            auto window = static_cast< HWND >( Application::getInstance().getWindow().getNativeWindow() );
            ScreenToClient( window, &point );
        }
        return DirectX::XMFLOAT2{ static_cast< float >( point.x ), static_cast< float >( point.y ) };
    }

    float Input::getMouseX()
    {
        auto mouse_position = getMousePosition();
        return mouse_position.x;
    }

    float Input::getMouseY()
    {
        auto mouse_position = getMousePosition();
        return mouse_position.y;
    }
}
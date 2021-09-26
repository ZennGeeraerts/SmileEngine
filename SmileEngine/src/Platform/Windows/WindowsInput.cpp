#include "smpch.h"
#include "WindowsInput.h"
#include "SmileEngine/Core/SmileGame.h"

namespace Smile
{
	Input* Input::m_pInstance = new WindowsInput{};

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto state = GetKeyState(keycode);
		return state & 0x8000;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		return IsKeyPressedImpl(button);
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		POINT point{};
		if (GetCursorPos(&point))
		{
			auto window = static_cast<HWND>(SmileGame::GetInstance().GetWindow().GetNativeWindow());
			ScreenToClient(window, &point);
		}
		return std::make_pair<float, float>
			(static_cast<float>(point.x), 
			static_cast<float>(point.y));
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto mousePosition = GetMousePositionImpl();
		return mousePosition.first;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto mousePosition = GetMousePositionImpl();
		return mousePosition.second;
	}
}
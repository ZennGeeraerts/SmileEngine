#pragma once

#include "SmileEngine/Core.h"

namespace Smile
{
	class SMILE_API Input
	{
	public:
		static bool IsKeyPressed(int keycode) { return m_pInstance->IsKeyPressedImpl(keycode); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

	private:
		static Input* m_pInstance;
	};

	Input* Input::m_pInstance = nullptr;
}

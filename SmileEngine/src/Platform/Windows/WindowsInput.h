#pragma once

#include "SmileEngine/Input.h"

namespace Smile
{
	class WindowsInput final : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) override;
	};
}


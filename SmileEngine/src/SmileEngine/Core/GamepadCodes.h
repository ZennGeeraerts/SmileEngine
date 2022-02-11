#pragma once
#include <cstdint>

namespace Smile
{
	using GamepadCode = uint16_t;

	namespace Gamepad
	{
		enum : GamepadCode
		{
			eButtonA				= 0xC3,
			eButtonB				= 0xC4,
			eButtonX				= 0xC5,
			eButtonY				= 0xC6,
			eRightShoulder			= 0xC7,
			eLeftShoulder			= 0xC8,
			eLeftTrigger			= 0xC9,
			eRightTrigger			= 0xCA,
			eDpadUp					= 0xCB,
			eDpadDown				= 0xCC,
			eDpadLeft				= 0xCD,
			eDpadRight				= 0xCE,
			eMenu					= 0xCF,
			eView					= 0xD0,
			eButtonLeftThumbStick	= 0xD1,
			eButtonRightThumbStick	= 0xD2,

			eLeftThumbStickUp		= 0xD3,
			eLeftThumbStickDown		= 0xD4,
			eLeftThumbStickRight	= 0xD5,
			eLeftThumbStickLeft		= 0xD6,
			eRightThumbStickUp		= 0xD7,
			eRightThumbStickDown	= 0xD8,
			eRightThumbStickRight	= 0xD9,
			eRightThumbStickLeft	= 0xDA
		};
	}
}

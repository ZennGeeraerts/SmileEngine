#pragma once

namespace smile::input
{
    using GamepadCode = Uint16;

	namespace gamepad
	{
		enum : GamepadCode
		{
			ButtonA					= 0xC3,
			ButtonB					= 0xC4,
			ButtonX					= 0xC5,
			ButtonY					= 0xC6,
			ShoulderRight			= 0xC7,
			ShoulderLeft			= 0xC8,
			TriggerLeft				= 0xC9,
			TriggerRight			= 0xCA,
			DpadUp					= 0xCB,
			DpadDown				= 0xCC,
			DpadLeft				= 0xCD,
			DpadRight				= 0xCE,
			Menu					= 0xCF,
			View					= 0xD0,
			ButtonThumbstickLeft	= 0xD1,
			ButtonThumbStickRight	= 0xD2,

			LeftThumbstickUp		= 0xD3,
			LeftThumbstickDown		= 0xD4,
			LeftThumbstickRight		= 0xD5,
			LeftThumbstickLeft		= 0xD6,
			RightThumbstickUp		= 0xD7,
			RightThumbstickDown		= 0xD8,
			RightThumbstickRight	= 0xD9,
			RightThumbstickLeft		= 0xDA
		};
	}
}

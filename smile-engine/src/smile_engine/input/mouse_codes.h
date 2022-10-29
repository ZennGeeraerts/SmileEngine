#pragma once

namespace Smile::Input
{
	using MouseCode = Uint16;

	namespace mouse
	{
		enum : MouseCode
		{
			ButtonLeft		= 0x01,
			ButtonRight		= 0x02,
			ButtonCancel	= 0x03,
			ButtonMiddle	= 0x04,
			ButtonX1		= 0x05,
			ButtonX2		= 0x06
		};
	}
}
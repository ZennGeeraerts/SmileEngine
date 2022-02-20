#pragma once
#include <cstdint>

namespace smile
{
	using MouseCode = uint16_t;

	namespace Mouse
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
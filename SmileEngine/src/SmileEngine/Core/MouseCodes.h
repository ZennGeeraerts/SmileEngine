#pragma once
#include <cstdint>

namespace Smile
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			eButtonLeft		= 0x01,
			eButtonRight	= 0x02,
			eButtonCancel	= 0x03,
			eButtonMiddle	= 0x04,
			eButtonX1		= 0x05,
			eButtonX2		= 0x06
		};
	}
}
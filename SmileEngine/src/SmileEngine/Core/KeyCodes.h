#pragma once
#include <cstdint>

// From Win32
namespace Smile
{
	namespace Key
	{
		using KeyCode = uint16_t;

		enum : KeyCode
		{
			eBack				= 0x08,
			eTab				= 0x09,
			eClear				= 0x0C,
			eEnter				= 0x0D,
			eShift				= 0x10,
			eControl			= 0x11,
			eAlt				= 0x12,
			ePause				= 0x13,
			eCapsLock			= 0x14,

			eKana				= 0x15,
			eHanguel			= 0x15,
			eHangul				= 0x15,
			eIMEOn				= 0x16,
			eJunja				= 0x17,
			eFinal				= 0x18,
			eHanja				= 0x19,
			eKanji				= 0x19,
			eIMEOff				= 0x1A,

			eEscape				= 0x1B,

			eSpace				= 0x20,

			ePageUp				= 0x21,
			ePageDown			= 0x22,
			eEnd				= 0x23,
			eHome				= 0x24,
			eLeft				= 0x25,
			eUp					= 0x26,
			eRight				= 0x27,
			eDown				= 0x28,
			eSelect				= 0x29,
			ePrint				= 0x2A,
			eExecute			= 0x2B,
			ePrintScreen		= 0x2C,
			eInsert				= 0x2D,
			eDelete				= 0x2E,
			eHelp				= 0x2F,

			D0					= 0x30,
			D1					= 0x31,
			D2					= 0x32,
			D3					= 0x33,
			D4					= 0x34,
			D5					= 0x35,
			D6					= 0x36,
			D7					= 0x37,
			D8					= 0x38,
			D9					= 0x39,

			A					= 0x41,
			B					= 0x42,
			C					= 0x43,
			D					= 0x44,
			E					= 0x45,
			F					= 0x46,
			G					= 0x47,
			H					= 0x48,
			I					= 0x49,
			J					= 0x4A,
			K					= 0x4B,
			L					= 0x4C,
			M					= 0x4D,
			N					= 0x4E,
			O					= 0x4F,
			P					= 0x50,
			Q					= 0x51,
			R					= 0x52,
			S					= 0x53,
			T					= 0x54,
			U					= 0x55,
			V					= 0x56,
			W					= 0x57,
			X					= 0x58,
			Y					= 0x59,
			Z					= 0x5A,

			eLeftWindows		= 0x5B,
			eRightWindows		= 0x5C,
			eApplications		= 0x5D,
			eSleep				= 0x5F,

			eNumPad0			= 0x60,
			eNumPad1			= 0x61,
			eNumPad2			= 0x62,
			eNumPad3			= 0x63,
			eNumPad4			= 0x64,
			eNumPad5			= 0x65,
			eNumPad6			= 0x66,
			eNumPad7			= 0x67,
			eNumPad8			= 0x68,
			eNumPad9			= 0x69,

			eNumPadMultiply		= 0x6A,
			eNumPadAdd			= 0x6B,
			eNumPadSeperate		= 0x6C,
			eNumPadSubtract		= 0x6D,
			eNumPadDecimal		= 0x6E,
			eNumPadDivide		= 0x6F,

			F1					= 0x70,
			F2					= 0x71,
			F3					= 0x72,
			F4					= 0x73,
			F5					= 0x74,
			F6					= 0x75,
			F7					= 0x76,
			F8					= 0x77,
			F9					= 0x78,
			F10					= 0x79,
			F11					= 0x7A,
			F12					= 0x7B,
			F13					= 0x7C,
			F14					= 0x7D,
			F15					= 0x7E,
			F16					= 0x7F,
			F17					= 0x80,
			F18					= 0x81,
			F19					= 0x82,
			F20					= 0x83,
			F21					= 0x84,
			F22					= 0x85,
			F23					= 0x86,
			F24					= 0x87,

			eNumLock			= 0x90,
			eScrollLock			= 0x91,

			eLeftShift			= 0xA0,
			eRightShift			= 0xA1,
			eLeftControl		= 0xA2,
			eRightControl		= 0xA3,
			eLeftAlt			= 0xA4,
			eRightAlt			= 0xA5,

			eBrowserBack		= 0xA6,
			eBrowserForward		= 0xA7,
			eBrowserRefresh		= 0xA8,
			eBrowserStop		= 0xA9,
			eBrowserSearch		= 0xAA,
			eBrowserFavorites	= 0xAB,
			eBrowserHome		= 0xAC,

			eVolumeMute			= 0xAD,
			eVolumeDown			= 0xAE,
			eVolumeUp			= 0xAF,

			eMediaNextTrack		= 0xB0,
			eMediaPrevTrack		= 0xB1,
			eMediaStop			= 0xB2,
			eMediaPlayPause		= 0xB3,

			eLaunchMail			= 0xB4,
			eLauchMediaSelect	= 0xB5,
			eLaunchApp1			= 0xB6,
			eLaunchApp2			= 0xB7,

			eOEM1				= 0xBA, // the ';:' key on a US standard keyboard
			ePlus				= 0xBB,
			eComma				= 0xBC,
			eMinus				= 0xBD,
			ePeriod				= 0xBE,
			eOEM2				= 0xBF, // the '/' key on a US standard keyboard
			eOEM3				= 0xC0, // the '~' key on a US standard keyboard

			eOEM4				= 0xDB, // the '[{' key on a US standard keyboard
			eOEM5				= 0xDC, // the '\|' key on a US standard keyboard
			eOEM6				= 0xDD, // the ']}' key on a US standard keyboard
			eOEM7				= 0xDE, // the 'single-quote/double-quote' key on a US standard keyboard
			eOEM8				= 0xDF,

			eOEM102				= 0xE2, // the '<>' key on a US standard keyboard

			eProcessKey			= 0xE5,

			ePacket				= 0xE7,
			
			eAttn				= 0xF6,
			eCrSel				= 0xF7,
			eExSel				= 0xF8,
			eEraseEOF			= 0xF9,
			ePlay				= 0xFA,
			eZoom				= 0xFB,
			eNoName				= 0xFC,
			ePA1				= 0xFD,
			eOEMClear			= 0xFE
		};
	}
}
#pragma once

// From Win32

#define SM_LMOUSEBUTTON        0x01
#define SM_RMOUSEBUTTON        0x02
#define SM_CANCEL         0x03
#define SM_MMOUSEBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#if(_WIN32_WINNT >= 0x0500)
#define SM_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define SM_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0x07 : reserved
 */


#define SM_BACK           0x08
#define SM_TAB            0x09

 /*
  * 0x0A - 0x0B : reserved
  */

#define SM_CLEAR          0x0C
#define SM_RETURN         0x0D

  /*
   * 0x0E - 0x0F : unassigned
   */

#define SM_SHIFT          0x10
#define SM_CONTROL        0x11
#define SM_MENU           0x12
#define SM_PAUSE          0x13
#define SM_CAPITAL        0x14

#define SM_KANA           0x15
#define SM_HANGEUL        0x15  /* old name - should be here for compatibility */
#define SM_HANGUL         0x15
#define SM_IME_ON         0x16
#define SM_JUNJA          0x17
#define SM_FINAL          0x18
#define SM_HANJA          0x19
#define SM_KANJI          0x19
#define SM_IME_OFF        0x1A

#define SM_ESCAPE         0x1B

#define SM_CONVERT        0x1C
#define SM_NONCONVERT     0x1D
#define SM_ACCEPT         0x1E
#define SM_MODECHANGE     0x1F

#define SM_SPACE          0x20
#define SM_PRIOR          0x21
#define SM_NEXT           0x22
#define SM_END            0x23
#define SM_HOME           0x24
#define SM_LEFT           0x25
#define SM_UP             0x26
#define SM_RIGHT          0x27
#define SM_DOWN           0x28
#define SM_SELECT         0x29
#define SM_PRINT          0x2A
#define SM_EXECUTE        0x2B
#define SM_SNAPSHOT       0x2C
#define SM_INSERT         0x2D
#define SM_DELETE         0x2E
#define SM_HELP           0x2F

   /*
	* SM_0 - SM_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
	* 0x3A - 0x40 : unassigned
	* SM_A - SM_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
	*/

#define SM_LWIN           0x5B
#define SM_RWIN           0x5C
#define SM_APPS           0x5D

	/*
	 * 0x5E : reserved
	 */

#define SM_SLEEP          0x5F

#define SM_NUMPAD0        0x60
#define SM_NUMPAD1        0x61
#define SM_NUMPAD2        0x62
#define SM_NUMPAD3        0x63
#define SM_NUMPAD4        0x64
#define SM_NUMPAD5        0x65
#define SM_NUMPAD6        0x66
#define SM_NUMPAD7        0x67
#define SM_NUMPAD8        0x68
#define SM_NUMPAD9        0x69
#define SM_MULTIPLY       0x6A
#define SM_ADD            0x6B
#define SM_SEPARATOR      0x6C
#define SM_SUBTRACT       0x6D
#define SM_DECIMAL        0x6E
#define SM_DIVIDE         0x6F
#define SM_F1             0x70
#define SM_F2             0x71
#define SM_F3             0x72
#define SM_F4             0x73
#define SM_F5             0x74
#define SM_F6             0x75
#define SM_F7             0x76
#define SM_F8             0x77
#define SM_F9             0x78
#define SM_F10            0x79
#define SM_F11            0x7A
#define SM_F12            0x7B
#define SM_F13            0x7C
#define SM_F14            0x7D
#define SM_F15            0x7E
#define SM_F16            0x7F
#define SM_F17            0x80
#define SM_F18            0x81
#define SM_F19            0x82
#define SM_F20            0x83
#define SM_F21            0x84
#define SM_F22            0x85
#define SM_F23            0x86
#define SM_F24            0x87

#if(_WIN32_WINNT >= 0x0604)

	 /*
	  * 0x88 - 0x8F : UI navigation
	  */

#define SM_NAVIGATION_VIEW     0x88 // reserved
#define SM_NAVIGATION_MENU     0x89 // reserved
#define SM_NAVIGATION_UP       0x8A // reserved
#define SM_NAVIGATION_DOWN     0x8B // reserved
#define SM_NAVIGATION_LEFT     0x8C // reserved
#define SM_NAVIGATION_RIGHT    0x8D // reserved
#define SM_NAVIGATION_ACCEPT   0x8E // reserved
#define SM_NAVIGATION_CANCEL   0x8F // reserved

#endif /* _WIN32_WINNT >= 0x0604 */

#define SM_NUMLOCK        0x90
#define SM_SCROLL         0x91

	  /*
	   * NEC PC-9800 kbd definitions
	   */
#define SM_OEM_NEC_EQUAL  0x92   // '=' key on numpad

	   /*
		* Fujitsu/OASYS kbd definitions
		*/
#define SM_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define SM_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define SM_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define SM_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define SM_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key

		/*
		 * 0x97 - 0x9F : unassigned
		 */

		 /*
		  * SM_L* & SM_R* - left and right Alt, Ctrl and Shift virtual keys.
		  * Used only as parameters to GetAsyncKeyState() and GetKeyState().
		  * No other API or message will distinguish left and right keys in this way.
		  */
#define SM_LSHIFT         0xA0
#define SM_RSHIFT         0xA1
#define SM_LCONTROL       0xA2
#define SM_RCONTROL       0xA3
#define SM_LMENU          0xA4
#define SM_RMENU          0xA5

#if(_WIN32_WINNT >= 0x0500)
#define SM_BROWSER_BACK        0xA6
#define SM_BROWSER_FORWARD     0xA7
#define SM_BROWSER_REFRESH     0xA8
#define SM_BROWSER_STOP        0xA9
#define SM_BROWSER_SEARCH      0xAA
#define SM_BROWSER_FAVORITES   0xAB
#define SM_BROWSER_HOME        0xAC

#define SM_VOLUME_MUTE         0xAD
#define SM_VOLUME_DOWN         0xAE
#define SM_VOLUME_UP           0xAF
#define SM_MEDIA_NEXT_TRACK    0xB0
#define SM_MEDIA_PREV_TRACK    0xB1
#define SM_MEDIA_STOP          0xB2
#define SM_MEDIA_PLAY_PAUSE    0xB3
#define SM_LAUNCH_MAIL         0xB4
#define SM_LAUNCH_MEDIA_SELECT 0xB5
#define SM_LAUNCH_APP1         0xB6
#define SM_LAUNCH_APP2         0xB7

#endif /* _WIN32_WINNT >= 0x0500 */

		  /*
		   * 0xB8 - 0xB9 : reserved
		   */

#define SM_OEM_1          0xBA   // ';:' for US
#define SM_OEM_PLUS       0xBB   // '+' any country
#define SM_OEM_COMMA      0xBC   // ',' any country
#define SM_OEM_MINUS      0xBD   // '-' any country
#define SM_OEM_PERIOD     0xBE   // '.' any country
#define SM_OEM_2          0xBF   // '/?' for US
#define SM_OEM_3          0xC0   // '`~' for US

		   /*
			* 0xC1 - 0xC2 : reserved
			*/

#if(_WIN32_WINNT >= 0x0604)

			/*
			 * 0xC3 - 0xDA : Gamepad input
			 */

#define SM_GAMEPAD_A                         0xC3 // reserved
#define SM_GAMEPAD_B                         0xC4 // reserved
#define SM_GAMEPAD_X                         0xC5 // reserved
#define SM_GAMEPAD_Y                         0xC6 // reserved
#define SM_GAMEPAD_RIGHT_SHOULDER            0xC7 // reserved
#define SM_GAMEPAD_LEFT_SHOULDER             0xC8 // reserved
#define SM_GAMEPAD_LEFT_TRIGGER              0xC9 // reserved
#define SM_GAMEPAD_RIGHT_TRIGGER             0xCA // reserved
#define SM_GAMEPAD_DPAD_UP                   0xCB // reserved
#define SM_GAMEPAD_DPAD_DOWN                 0xCC // reserved
#define SM_GAMEPAD_DPAD_LEFT                 0xCD // reserved
#define SM_GAMEPAD_DPAD_RIGHT                0xCE // reserved
#define SM_GAMEPAD_MENU                      0xCF // reserved
#define SM_GAMEPAD_VIEW                      0xD0 // reserved
#define SM_GAMEPAD_LEFT_THUMBSTICK_BUTTON    0xD1 // reserved
#define SM_GAMEPAD_RIGHT_THUMBSTICK_BUTTON   0xD2 // reserved
#define SM_GAMEPAD_LEFT_THUMBSTICK_UP        0xD3 // reserved
#define SM_GAMEPAD_LEFT_THUMBSTICK_DOWN      0xD4 // reserved
#define SM_GAMEPAD_LEFT_THUMBSTICK_RIGHT     0xD5 // reserved
#define SM_GAMEPAD_LEFT_THUMBSTICK_LEFT      0xD6 // reserved
#define SM_GAMEPAD_RIGHT_THUMBSTICK_UP       0xD7 // reserved
#define SM_GAMEPAD_RIGHT_THUMBSTICK_DOWN     0xD8 // reserved
#define SM_GAMEPAD_RIGHT_THUMBSTICK_RIGHT    0xD9 // reserved
#define SM_GAMEPAD_RIGHT_THUMBSTICK_LEFT     0xDA // reserved

#endif /* _WIN32_WINNT >= 0x0604 */


#define SM_OEM_4          0xDB  //  '[{' for US
#define SM_OEM_5          0xDC  //  '\|' for US
#define SM_OEM_6          0xDD  //  ']}' for US
#define SM_OEM_7          0xDE  //  ''"' for US
#define SM_OEM_8          0xDF

			 /*
			  * 0xE0 : reserved
			  */

			  /*
			   * Various extended or enhanced keyboards
			   */
#define SM_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define SM_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define SM_ICO_HELP       0xE3  //  Help key on ICO
#define SM_ICO_00         0xE4  //  00 key on ICO

#if(WINVER >= 0x0400)
#define SM_PROCESSKEY     0xE5
#endif /* WINVER >= 0x0400 */

#define SM_ICO_CLEAR      0xE6


#if(_WIN32_WINNT >= 0x0500)
#define SM_PACKET         0xE7
#endif /* _WIN32_WINNT >= 0x0500 */

			   /*
				* 0xE8 : unassigned
				*/

				/*
				 * Nokia/Ericsson definitions
				 */
#define SM_OEM_RESET      0xE9
#define SM_OEM_JUMP       0xEA
#define SM_OEM_PA1        0xEB
#define SM_OEM_PA2        0xEC
#define SM_OEM_PA3        0xED
#define SM_OEM_WSCTRL     0xEE
#define SM_OEM_CUSEL      0xEF
#define SM_OEM_ATTN       0xF0
#define SM_OEM_FINISH     0xF1
#define SM_OEM_COPY       0xF2
#define SM_OEM_AUTO       0xF3
#define SM_OEM_ENLW       0xF4
#define SM_OEM_BACKTAB    0xF5

#define SM_ATTN           0xF6
#define SM_CRSEL          0xF7
#define SM_EXSEL          0xF8
#define SM_EREOF          0xF9
#define SM_PLAY           0xFA
#define SM_ZOOM           0xFB
#define SM_NONAME         0xFC
#define SM_PA1            0xFD
#define SM_OEM_CLEAR      0xFE
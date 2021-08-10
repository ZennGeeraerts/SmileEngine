#pragma once

#ifdef SM_PLATFORM_WINDOWS
	#ifdef  SM_BUILD_DLL
		#define SMILE_API __declspec(dllexport)
	#else
		#define SMILE_API __declspec(dllimport)
	#endif
#else
#error SmileEngine only supports Windows
#endif

#define	BIT(x) (1 << x)
#define SAFE_DELETE(p) if (p) { delete (p); (p) = nullptr; }
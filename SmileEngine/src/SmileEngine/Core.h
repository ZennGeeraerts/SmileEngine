#pragma once

#include <memory>

#ifdef SM_PLATFORM_WINDOWS
#if SM_DYNAMIC_LINK
	#ifdef  SM_BUILD_DLL
		#define SMILE_API __declspec(dllexport)
	#else
		#define SMILE_API __declspec(dllimport)
	#endif
#else
	#define SMILE_API
#endif
#else
	#error SmileEngine only supports Windows
#endif

#ifdef SM_LOG_DEBUG
	#define SM_ENABLE_ASSERTS
#endif

#ifdef SM_ENABLE_ASSERTS
	#define SM_ASSERT(x, ...) { if(!(x)) { SM_LOG_ERROR("Assertion Failed: %s", __VA_ARGS__); __debugbreak(); } }
#else
	#define SM_ASSERT(x, ...)
#endif

#define	BIT(x) (1 << x)
#define SM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define SAFE_DELETE(p) if (p) { delete (p); (p) = nullptr; }
#define SAFE_RELEASE(r) if (r) { r->Release(); (r) = nullptr; }

namespace Smile
{
	template<typename Type>
	using Scope = std::unique_ptr<Type>;

	template<typename Type>
	using Ref = std::shared_ptr<Type>;
}
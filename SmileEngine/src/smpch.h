#pragma once

// Data structures
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#include "SmileEngine/Core/Timestep.h"
#include "SmileEngine/Core/SmTime.h"

#include <DirectXMath.h>

// Platform specific headers
#ifdef SM_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <windowsx.h>

	#include <dxgi.h>
	#include <d3d11.h>
	#include <d3dcompiler.h>
	#include <d3dx11effect.h>
	#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment(lib, "dx11effects_d.lib")
	#else 
	#pragma comment(lib, "dx11effects.lib")
	#endif
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/core/platform_detection.h"

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

#include "smile_engine/core/timestep.h"
#include "smile_engine/core/timer.h"

#include <DirectXMath.h>

// Platform specific headers
#ifdef SM_PLATFORM_WINDOWS
#    define NOMINMAX
#    include <Windows.h>
#    include <windowsx.h>

#    if defined( DEBUG ) || defined( _DEBUG )
#        pragma comment( lib, "dx11effects_d.lib" )
#    else
#        pragma comment( lib, "dx11effects.lib" )
#    endif
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile/common/platform/executable_path.h"

#include <Windows.h>

namespace smile::platform
{
    std::filesystem::path GetExecutablePath()
    {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW( NULL, path, MAX_PATH );
        return path;
    }
}
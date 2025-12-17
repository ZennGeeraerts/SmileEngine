/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile/common/platform/executable_path.h"

#include <Windows.h>

namespace smile::platform
{
    fs::Path GetExecutablePath()
    {
        wchar_t path[MAX_PATH];
        const DWORD length = GetModuleFileNameW( NULL, path, MAX_PATH );
        if ( length == 0 )
            return {};

        primitive::Vector< char > buffer( MAX_PATH );

        const int convertedLen =
            WideCharToMultiByte( CP_ACP, 0, path, length, buffer.GetData(), MAX_PATH, nullptr, nullptr );

        if ( convertedLen <= 0 )
            return {};

        return primitive::StringView{ buffer.GetData(), static_cast< Count >( convertedLen ) };
    }
}
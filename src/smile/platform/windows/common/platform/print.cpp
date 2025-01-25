/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "platform/print.h"

#include <Windows.h>

namespace smile::platform
{
    void Print( const char *text )
    {
        OutputDebugStringA( ( std::string{ text } + '\n' ).c_str() );
        puts( text );
    }
}
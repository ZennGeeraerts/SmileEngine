/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "platform/print.h"

#include <Windows.h>
#include <io.h>

namespace smile::platform
{
    static HANDLE s_pHandle = reinterpret_cast< HANDLE >( ::_get_osfhandle( ::_fileno( stdout ) ) );

    void Print( const char *text, Uint32 length )
    {
        DWORD bytesWritten = 0;
        ::WriteFile( s_pHandle, text, length, &bytesWritten, nullptr );

        ::fflush( stdout );
    }

    const char *EOL()
    {
        return "\r\n";
    }
}
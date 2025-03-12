/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile/common/platform/print.h"

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

    void SetConsoleColor( Uint16 attributes )
    {
        CONSOLE_SCREEN_BUFFER_INFO originalBufferInfo;
        ::GetConsoleScreenBufferInfo( s_pHandle, &originalBufferInfo );

        auto newAttributes = static_cast< WORD >( attributes ) | ( originalBufferInfo.wAttributes & 0xfff0 );
        auto ignored = ::SetConsoleTextAttribute( s_pHandle, static_cast< WORD >( newAttributes ) );
        ( void )( ignored );
    }
}
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

    // Set the console color and return the original color
    Uint16 SetConsoleColor( Uint16 attributes )
    {
        CONSOLE_SCREEN_BUFFER_INFO originalBufferInfo;
        if ( !::GetConsoleScreenBufferInfo( s_pHandle, &originalBufferInfo ) )
            return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // return white when getting screen buffer fails

        auto newAttributes = static_cast< WORD >( attributes ) | ( originalBufferInfo.wAttributes & 0xfff0 );
        [[maybe_unused]] bool ignored = ::SetConsoleTextAttribute( s_pHandle, static_cast< WORD >( newAttributes ) );
        return static_cast< Uint16 >( originalBufferInfo.wAttributes );
    }
}
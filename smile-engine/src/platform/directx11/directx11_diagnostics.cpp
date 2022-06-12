#include "smpch.h"
#include "directx11_diagnostics.h"

#include <d3d11.h>

namespace smile::renderer
{
    LPTSTR getDirectX11ErrorMessage( HRESULT result )
    {
        // This works only with windows 8 or higher
        // FormatMessage are since then including DirectX
        // Before the library DXERR was used
        // TODO: check if the user is on windows 8 or higher

        LPTSTR error_text = NULL;
        FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            result,
            MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
            ( LPTSTR )&error_text,
            0,
            NULL );

        return error_text;
    }
}
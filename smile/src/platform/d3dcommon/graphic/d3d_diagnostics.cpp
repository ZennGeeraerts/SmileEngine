/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "d3d_diagnostics.h"

#include "platform/windows/common/platform/utils.h"

namespace smile::graphic::rhi
{
    LPTSTR GetD3DErrorMessage( HRESULT result )
    {
        // This works only with windows 8 or higher
        // FormatMessage are since then including DirectX
        // Before the library DXERR was used
        // TODO: check if the user is on windows 8 or higher

        LPTSTR errorText = NULL;
        FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            result,
            MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
            ( LPTSTR )&errorText,
            0,
            NULL );

        return errorText;
    }

    primitive::String GetD3DErrorMessageString( HRESULT result )
    {
        LPTSTR lpstr = GetD3DErrorMessage( result );
        const char *cstr = platform::LPTSTRToCString( lpstr );
        return primitive::String{ cstr };
    }
}
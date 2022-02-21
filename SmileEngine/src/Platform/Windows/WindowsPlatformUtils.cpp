#include "smpch.h"

#include "SmileEngine/Utils/PlatformUtils.h"
#include "SmileEngine/Core/Application.h"

#include <commdlg.h>

namespace smile::utils
{
    std::string OpenFile( const char *pFilter )
    {
        OPENFILENAMEA openFileName{};
        CHAR szFile[260] = { 0 };
        ZeroMemory( &openFileName, sizeof( OPENFILENAMEA ) );
        openFileName.lStructSize = sizeof( OPENFILENAMEA );
        openFileName.hwndOwner = static_cast< HWND >( Application::GetInstance().GetWindow().GetNativeWindow() );
        openFileName.lpstrFile = szFile;
        openFileName.nMaxFile = sizeof( szFile );
        openFileName.lpstrFilter = pFilter;
        openFileName.nFilterIndex = 1;
        openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if ( GetOpenFileNameA( &openFileName ) == TRUE )
            return openFileName.lpstrFile;

        return std::string{};
    }

    std::string SaveFile( const char *pFilter )
    {
        OPENFILENAMEA openFileName{};
        CHAR szFile[260] = { 0 };
        ZeroMemory( &openFileName, sizeof( OPENFILENAMEA ) );
        openFileName.lStructSize = sizeof( OPENFILENAMEA );
        openFileName.hwndOwner = static_cast< HWND >( Application::GetInstance().GetWindow().GetNativeWindow() );
        openFileName.lpstrFile = szFile;
        openFileName.nMaxFile = sizeof( szFile );
        openFileName.lpstrFilter = pFilter;
        openFileName.nFilterIndex = 1;
        openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        // Destract the extention from the filter
        openFileName.lpstrDefExt = strchr( pFilter, '\0' ) + 1;

        if ( GetSaveFileNameA( &openFileName ) == TRUE )
            return openFileName.lpstrFile;

        return std::string{};
    }
}
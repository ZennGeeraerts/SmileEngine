#include "smpch.h"

#include "smile_engine/utils/platform_utils.h"
#include "smile_engine/core/application.h"

#include <commdlg.h>

namespace smile::utils
{
    std::string openFile( const char *filter )
    {
        OPENFILENAMEA open_file_name{};
        CHAR sz_file[260] = { 0 };
        ZeroMemory( &open_file_name, sizeof( OPENFILENAMEA ) );
        open_file_name.lStructSize = sizeof( OPENFILENAMEA );
        open_file_name.hwndOwner = static_cast< HWND >( Application::getInstance().getWindow().getNativeWindow() );
        open_file_name.lpstrFile = sz_file;
        open_file_name.nMaxFile = sizeof( sz_file );
        open_file_name.lpstrFilter = filter;
        open_file_name.nFilterIndex = 1;
        open_file_name.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if ( GetOpenFileNameA( &open_file_name ) == TRUE )
            return open_file_name.lpstrFile;

        return std::string{};
    }

    std::string saveFile( const char *filter )
    {
        OPENFILENAMEA open_file_name{};
        CHAR sz_file[260] = { 0 };
        ZeroMemory( &open_file_name, sizeof( OPENFILENAMEA ) );
        open_file_name.lStructSize = sizeof( OPENFILENAMEA );
        open_file_name.hwndOwner = static_cast< HWND >( Application::getInstance().getWindow().getNativeWindow() );
        open_file_name.lpstrFile = sz_file;
        open_file_name.nMaxFile = sizeof( sz_file );
        open_file_name.lpstrFilter = filter;
        open_file_name.nFilterIndex = 1;
        open_file_name.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        // Destract the extention from the filter
        open_file_name.lpstrDefExt = strchr( filter, '\0' ) + 1;

        if ( GetSaveFileNameA( &open_file_name ) == TRUE )
            return open_file_name.lpstrFile;

        return std::string{};
    }
}
/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"

#include "engine/core/window/file_dialog.h"
#include "engine/core/application/application.h"

#include <Windows.h>
#include <commdlg.h>

namespace smile::window
{
    std::string FileDialog::OpenFile( const char *filter )
    {
        OPENFILENAMEA openFileName{};
        CHAR szFile[260] = { 0 };
        ZeroMemory( &openFileName, sizeof( OPENFILENAMEA ) );
        openFileName.lStructSize = sizeof( OPENFILENAMEA );
        openFileName.hwndOwner =
            static_cast< HWND >( application::Application::GetInstance().GetMainWindow().GetNativeWindow() );
        openFileName.lpstrFile = szFile;
        openFileName.nMaxFile = sizeof( szFile );
        openFileName.lpstrFilter = filter;
        openFileName.nFilterIndex = 1;
        openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if ( GetOpenFileNameA( &openFileName ) == TRUE )
            return openFileName.lpstrFile;

        return std::string{};
    }

    std::string FileDialog::SaveFile( const char *filter )
    {
        OPENFILENAMEA openFileName{};
        CHAR szFile[260] = { 0 };
        ZeroMemory( &openFileName, sizeof( OPENFILENAMEA ) );
        openFileName.lStructSize = sizeof( OPENFILENAMEA );
        openFileName.hwndOwner =
            static_cast< HWND >( application::Application::GetInstance().GetMainWindow().GetNativeWindow() );
        openFileName.lpstrFile = szFile;
        openFileName.nMaxFile = sizeof( szFile );
        openFileName.lpstrFilter = filter;
        openFileName.nFilterIndex = 1;
        openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        // Destract the extention from the filter
        openFileName.lpstrDefExt = strchr( filter, '\0' ) + 1;

        if ( GetSaveFileNameA( &openFileName ) == TRUE )
            return openFileName.lpstrFile;

        return std::string{};
    }
}
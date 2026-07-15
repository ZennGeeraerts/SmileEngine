/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/application/application.h"

#ifdef SM_PLATFORM_WINDOWS

#    include <Windows.h>

extern smile::application::Application *smile::application::CreateApplication(
    smile::application::ApplicationCommandLineArgs commandLineArgs );

#    pragma warning( push )
#    pragma warning( disable : 6387 )
int main( int argc, char **argv )
{
    return WinMain( GetModuleHandle( NULL ), NULL, NULL, SW_SHOW );
}
#    pragma warning( pop )

#    pragma warning( push )
#    pragma warning( disable : 28251 )
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow )
{
    smile::application::ApplicationCommandLineArgs commandLineArgs{ __argc, __argv };
    smile::application::Application *pApplication = smile::application::CreateApplication( commandLineArgs );
    pApplication->Initialize();
    pApplication->Run();
    delete pApplication;
    return 0;
}
#    pragma warning( pop )
#endif
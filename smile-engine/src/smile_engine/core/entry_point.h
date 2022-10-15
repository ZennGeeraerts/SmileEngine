#pragma once

#ifdef SM_PLATFORM_WINDOWS

#    include <Windows.h>

extern smile::Application *smile::createApplication( smile::ApplicationCommandLineArgs command_line_args );

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
    smile::Application *application = smile::createApplication( smile::ApplicationCommandLineArgs{} );
    application->run();
    delete application;
    return 0;
}
#    pragma warning( pop )
#endif
#pragma once

#ifdef SM_PLATFORM_WINDOWS

#    include <Windows.h>

extern Smile::Application *Smile::CreateApplication( Smile::ApplicationCommandLineArgs commandLineArgs );

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
    Smile::Application *pApplication = Smile::CreateApplication( Smile::ApplicationCommandLineArgs{} );
    pApplication->Run();
    delete pApplication;
    return 0;
}
#    pragma warning( pop )
#endif
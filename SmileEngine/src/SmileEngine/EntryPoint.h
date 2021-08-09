#pragma once

#ifdef SM_PLATFORM_WINDOWS

extern Smile::Application* Smile::CreateApplication();

void main(int argc, char** argv)
{
	Smile::Application* pApp = Smile::CreateApplication();
	pApp->Run();
	delete pApp;
}

#endif
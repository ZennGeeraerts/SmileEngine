#pragma once

#ifdef SM_PLATFORM_WINDOWS

extern Smile::SmileGame* Smile::CreateGame();

void main(int argc, char** argv)
{
	SM_WARNING("Initialized engine");
	SM_INFO("Creating application");

	Smile::SmileGame* pApp = Smile::CreateGame();

	SM_WARNING("Application created");
	SM_WARNING("Running Application");

	pApp->Run();

	SM_WARNING("Application stopped");

	delete pApp;
}

#endif
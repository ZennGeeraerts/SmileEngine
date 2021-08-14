#include "MainGame.h"

Smile::SmileGame* Smile::CreateGame()
{
	// This application will get passed to the entry point of the engine
	// and will be deleted once the engine closes
	return new MainGame();
}

MainGame::MainGame()
{
	PushLayer(new ExampleLayer{});
	PushOverlay(new Smile::ImGuiLayer{});
}

MainGame::~MainGame()
{

}
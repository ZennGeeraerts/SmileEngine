#pragma once
#include <SmileEngine.h>

class ExampleLayer : public Smile::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//SM_INFO("ExampleLayer::Update");

		if (Smile::Input::IsKeyPressed(SM_TAB))
			SM_LOG_TRACE("Tab key is pressed!");
	}

	void OnEvent(Smile::Event& event) override
	{
		if (event.GetEventType() == Smile::EventType::eKeyPressed)
		{
			Smile::KeyPressedEvent& e = static_cast<Smile::KeyPressedEvent&>(event);
			SM_LOG_TRACE("%c", static_cast<unsigned char>(e.GetKeyCode()));
		}
	}
};

class MainGame final : public Smile::SmileGame
{
public:
	MainGame();
	~MainGame();
};
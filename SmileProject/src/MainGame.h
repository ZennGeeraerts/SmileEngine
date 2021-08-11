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
		SM_INFO("ExampleLayer::Update");
	}

	void OnEvent(Smile::Event& event) override
	{
		SM_TRACE("%s", event.ToString().c_str());
	}
};

class MainGame final : public Smile::SmileGame
{
public:
	MainGame();
	~MainGame();
};
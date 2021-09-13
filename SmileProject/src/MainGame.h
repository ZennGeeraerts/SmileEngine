#pragma once
#include <SmileEngine.h>

class ExampleLayer final : public Smile::Layer
{
public:
	ExampleLayer();

	virtual void OnUpdate(Smile::Timestep deltaTime) override;
	virtual void OnEvent(Smile::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	Smile::Ref<Smile::Scene> m_pActiveScene;

	Smile::Entity m_CameraEntity;
	DirectX::XMFLOAT3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.f;
	float m_CameraRotation = 0.f;
	float m_CameraRotationSpeed = 180.f;

	Smile::Entity m_Gun;
};

class MainGame final : public Smile::SmileGame
{
public:
	MainGame();
	~MainGame();
};
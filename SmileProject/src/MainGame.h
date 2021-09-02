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
	Smile::Ref<Smile::Shader> m_pShader;
	Smile::Ref<Smile::VertexBuffer> m_pVertexBuffer;
	Smile::Ref<Smile::IndexBuffer> m_pIndexBuffer;

	Smile::OrthographicCamera m_Camera;
	DirectX::XMFLOAT3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.f;
	float m_CameraRotation = 0.f;
	float m_CameraRotationSpeed = 180.f;
	DirectX::XMFLOAT3 m_TriangleColor = { 0, 0, 0 };
};

class MainGame final : public Smile::SmileGame
{
public:
	MainGame();
	~MainGame();
};
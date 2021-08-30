#pragma once
#include <SmileEngine.h>

class ExampleLayer final : public Smile::Layer
{
public:
	ExampleLayer();

	virtual void OnUpdate() override;
	virtual void OnEvent(Smile::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	std::shared_ptr<Smile::Shader> m_pShader;
	std::shared_ptr<Smile::VertexBuffer> m_pVertexBuffer;
	std::shared_ptr<Smile::IndexBuffer> m_pIndexBuffer;

	Smile::OrthographicCamera m_Camera;
	DirectX::XMFLOAT3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.001f;
	float m_CameraRotation = 0.f;
	float m_CameraRotationSpeed = 0.005f;
};

class MainGame final : public Smile::SmileGame
{
public:
	MainGame();
	~MainGame();
};
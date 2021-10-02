#pragma once
#include <SmileEngine.h>

namespace Smile
{
	class SmileEditorLayer final : public Layer
	{
	public:
		SmileEditorLayer();
		virtual ~SmileEditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		Ref<Scene> m_pActiveScene;

		Entity m_CameraEntity;
		float m_CameraMoveSpeed = 5.f;
		float m_CameraRotationSpeed = 180.f;

		Entity m_GunEntity;

		Ref<Framebuffer> m_pFramebuffer;

		DirectX::XMFLOAT2 m_ViewportSize = { 0.f, 0.f };
		bool m_bViewportFocused = false;
		bool m_bViewportHovered = false;
	};

	class SmileEditorGame final : public SmileGame
	{
	public:
		SmileEditorGame();
		virtual ~SmileEditorGame() = default;
	};
}


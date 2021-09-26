#pragma once
#include "SmileEngine/Renderer/PerspectiveCamera.h"
#include "SmileEngine/Core/Timestep.h"

#include "SmileEngine/Events/ApplicationEvent.h"
#include "SmileEngine/Events/MouseEvent.h"

#include "SmileEngine/Scene/Components.h"

namespace Smile
{
	class PerspectiveCameraController final
	{
	public:
		PerspectiveCameraController(float fov, float aspectRatio);

		void OnUpdate(Timestep deltaTime);
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResizedEvent(WindowResizeEvent& e);

	private:
		float m_FOV = 45.f;
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		PerspectiveCamera m_Camera;

		DirectX::XMFLOAT3 m_CameraPosition = { 0.f, 0.f, 0.f };
		DirectX::XMFLOAT3 m_CameraRotation = { 0.f, 0.f, 0.f };
		float m_CameraMoveSpeed = m_ZoomLevel;
		float m_CameraRotationSpeed = 180.f;
	};
}


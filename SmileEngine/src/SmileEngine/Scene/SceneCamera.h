#pragma once
#include "SmileEngine/Renderer/Camera.h"

namespace Smile
{
	class SceneCamera final : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetPerspectiveCamera(float fov, float nearPlane, float farPlane);
		void SetViewportSize(uint32_t width, uint32_t height);

		void SetFOV(float fov) { m_FOV = fov; RecalculateProjectionMatrix(); }
		float GetFOV() const { return m_FOV; }

	private:
		void RecalculateProjectionMatrix();

	private:
		float m_FOV = 45.f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 2500.f;

		float m_AspectRatio = 1.0f;
	};
}

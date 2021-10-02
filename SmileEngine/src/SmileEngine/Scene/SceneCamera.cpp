#include "smpch.h"
#include "SceneCamera.h"

namespace Smile
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetPerspectiveCamera(float fov, float nearPlane, float farPlane)
	{
		m_FOV = fov;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = width / static_cast<float>(height);

		RecalculateProjectionMatrix();
	}

	void SceneCamera::RecalculateProjectionMatrix()
	{
		DirectX::XMMATRIX projectionMatrixMat = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
		DirectX::XMFLOAT4X4 projectionMatrix{};
		DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrixMat);
	}
}
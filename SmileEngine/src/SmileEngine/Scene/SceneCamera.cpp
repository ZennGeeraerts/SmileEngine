#include "smpch.h"
#include "SceneCamera.h"

namespace Smile
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = width / static_cast<float>(height);

		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetPerspectiveCamera(float fov, float nearPlane, float farPlane)
	{
		m_ProjectionType = ProjectionType::ePerspective;
		m_FOV = fov;
		m_PerspectiveNearPlane = nearPlane;
		m_PerspectiveFarPlane = farPlane;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetOrthographicCamera(float size, float nearPlane, float farPlane)
	{
		m_ProjectionType = ProjectionType::eOrthographic;
		m_Size = size;
		m_PerspectiveNearPlane = nearPlane;
		m_PerspectiveFarPlane = farPlane;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::RecalculateProjectionMatrix()
	{
		switch (m_ProjectionType)
		{
		case ProjectionType::ePerspective:
		{
			DirectX::XMMATRIX projectionMatrixMat = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_FOV), m_AspectRatio, m_PerspectiveNearPlane, m_PerspectiveFarPlane);
			DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrixMat);
			break;
		}
		case ProjectionType::eOrthographic:
		{
			float orthoLeft = -m_Size * m_AspectRatio * 0.5f;
			float orthoRight = m_Size * m_AspectRatio * 0.5f;
			float orthoBottom = -m_Size * 0.5f;
			float orthoTop = m_Size * 0.5f;

			DirectX::XMMATRIX projectionMatrixMat = DirectX::XMMatrixOrthographicOffCenterLH(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNearPlane, m_OrthographicFarPlane);
			DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrixMat);
			break;
		}
		}
	}
}
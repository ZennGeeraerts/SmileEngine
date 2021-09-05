#include "smpch.h"
#include "PerspectiveCamera.h"

namespace Smile
{
	using namespace DirectX;

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio)
	{
		XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, 2500.f));

		XMStoreFloat4x4(&m_ViewMatrix, DirectX::XMMatrixIdentity());
		XMStoreFloat4x4(&m_ViewProjectionMatrix, XMLoadFloat4x4(&m_ViewMatrix) * XMLoadFloat4x4(&m_ProjectionMatrix));
	}

	void PerspectiveCamera::SetPosition(const DirectX::XMFLOAT3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetRotation(float rotation)
	{
		m_Rotation = rotation;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		XMMATRIX transform = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(m_Rotation)), XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z));
		XMStoreFloat4x4(&m_ViewMatrix, XMMatrixInverse(nullptr, transform));

		XMStoreFloat4x4(&m_ViewProjectionMatrix, XMLoadFloat4x4(&m_ViewMatrix) * XMLoadFloat4x4(&m_ProjectionMatrix));
	}
}
#pragma once
#include <DirectXMath.h>

namespace Smile
{
	class Camera final
	{
	public:
		Camera()
		{
			DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, DirectX::XMMatrixIdentity());
		}

		Camera(const DirectX::XMFLOAT4X4& projectionMatrix)
			: m_ProjectionMatrix{ projectionMatrix }
		{}

		Camera(const DirectX::XMMATRIX& projectionMatrix)
		{
			DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrix);
		}

		virtual ~Camera() = default;

		void SetProjectionMatrix(const DirectX::XMFLOAT4X4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }
		const DirectX::XMFLOAT4X4& GetProjectionMatrix() const { return m_ProjectionMatrix; };

	private:
		DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	};
}
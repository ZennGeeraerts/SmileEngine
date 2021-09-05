#pragma once
#include <DirectXMath.h>

namespace Smile
{
	class Camera
	{
	public:
		Camera(const DirectX::XMFLOAT4X4& projectionMatrix)
			: m_ProjectionMatrix{ projectionMatrix }
		{}

		Camera(const DirectX::XMMATRIX& projectionMatrix)
		{
			DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrix);
		}

		virtual ~Camera() = default;

		const DirectX::XMFLOAT4X4& GetProjectionMatrix() const { return m_ProjectionMatrix; };

	private:
		DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	};
}
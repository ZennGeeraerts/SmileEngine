#pragma once

#include "SmileEngine/Core.h"

namespace Smile
{
	class SMILE_API OrthographicCamera final
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const DirectX::XMFLOAT3& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }
		const DirectX::XMFLOAT4X4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const DirectX::XMFLOAT4X4& GetViewMatrix() const { return m_ViewMatrix; }
		const DirectX::XMFLOAT4X4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void SetPosition(const DirectX::XMFLOAT3& position);
		void SetRotation(float rotation);

	private:
		void RecalculateViewMatrix();

	private:
		DirectX::XMFLOAT4X4 m_ProjectionMatrix;
		DirectX::XMFLOAT4X4 m_ViewMatrix;
		DirectX::XMFLOAT4X4 m_ViewProjectionMatrix;

		DirectX::XMFLOAT3 m_Position;
		float m_Rotation = 0.0f; // the Z rotation of the camera
	};
}


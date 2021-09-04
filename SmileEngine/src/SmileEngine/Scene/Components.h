#pragma once
#include <DirectXMath.h>

namespace Smile
{
	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag{ tag }
		{}

		std::string Tag;
	};

	struct TransformComponent final
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const DirectX::XMFLOAT3& translation, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& scale)
			: Translation{ translation }
			, Rotation{ rotation }
			, Scale{ scale }
		{}

		DirectX::XMFLOAT4X4 GetTransform() const
		{
			DirectX::XMMATRIX transformMat = DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z) * DirectX::XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z)
				* DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);

			DirectX::XMFLOAT4X4 transform{};
			DirectX::XMStoreFloat4x4(&transform, transformMat);
			return transform;
		}

		DirectX::XMFLOAT3 Translation{ 0.f, 0.f, 0.f };
		DirectX::XMFLOAT3 Rotation{ 0.f, 0.f, 0.f };
		DirectX::XMFLOAT3 Scale{ 1.f, 1.f, 1.f };
	};

	struct MeshRendererComponent final
	{
		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;
		MeshRendererComponent(const DirectX::XMFLOAT3& color)
			: Color{ color }
		{}

		DirectX::XMFLOAT3 Color{ 1.f, 1.f, 1.f };
	};
}
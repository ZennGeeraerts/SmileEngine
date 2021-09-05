#pragma once
#include <DirectXMath.h>
#include "SmileEngine/Renderer/Shader.h"
#include "SmileEngine/Renderer/Camera.h"

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
		struct MeshRendererData final
		{
			void* pVertices = nullptr;
			uint32_t VertexCount{};
			uint32_t* pIndices = nullptr;
			uint32_t IndexCount{};
			BufferLayout BufferLayout{};
			std::string ShaderFilePath{};
		};

		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;
		MeshRendererComponent(RenderingContext* pContext, const MeshRendererData& meshRendererData)
		{
			pVertexBuffer.reset(VertexBuffer::Create(pContext, meshRendererData.pVertices, meshRendererData.VertexCount, meshRendererData.BufferLayout));
			pIndexBuffer.reset(IndexBuffer::Create(pContext, meshRendererData.pIndices, meshRendererData.IndexCount));
			pShader.reset(Shader::Create(pContext, meshRendererData.ShaderFilePath, meshRendererData.BufferLayout));
		}

		Ref<VertexBuffer> pVertexBuffer = nullptr;
		Ref<IndexBuffer> pIndexBuffer = nullptr;
		Ref<Shader> pShader = nullptr;
	};

	struct CameraComponent final
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const DirectX::XMFLOAT4X4& projectionMatrix)
			: Camera{ projectionMatrix }
		{}
		CameraComponent(const DirectX::XMMATRIX& projectionMatrix)
			: Camera{ projectionMatrix }
		{}

		Smile::Camera Camera;
		bool bPrimary = true;
	};
}
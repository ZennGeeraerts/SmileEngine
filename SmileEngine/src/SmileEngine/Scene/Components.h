#pragma once
#include <DirectXMath.h>

#include "SmileEngine/Renderer/Buffer.h"
#include "SmileEngine/Renderer/Shader.h"

#include "SmileEngine/Renderer/Camera.h"

#include "SmileEngine/Core/MeshLoader.h"
#include "SmileEngine/Renderer/Mesh.h"

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

		DirectX::XMFLOAT3 GetForward()
		{
			/*DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
			DirectX::XMFLOAT3 forward{};
			DirectX::XMVECTOR forwardVec = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0, 0, 1, 0), rotationMat);
			DirectX::XMStoreFloat3(&forward, forwardVec);
			return forward;*/
			DirectX::XMFLOAT3 forward{ 0, 0, 1 };
			RotateVector(forward);
			return forward;
		}

		DirectX::XMFLOAT3 GetRight()
		{
			/*DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
			DirectX::XMFLOAT3 right{};
			DirectX::XMVECTOR rightVec = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0, 0, 1, 0), rotationMat);
			DirectX::XMStoreFloat3(&right, rightVec);
			return right;*/
			DirectX::XMFLOAT3 right{ 1, 0, 0 };
			RotateVector(right);
			return right;
		}

		DirectX::XMFLOAT3 Translation{ 0.f, 0.f, 0.f };
		DirectX::XMFLOAT3 Rotation{ 0.f, 0.f, 0.f };
		DirectX::XMFLOAT3 Scale{ 1.f, 1.f, 1.f };

	private:
		void RotateVector(DirectX::XMFLOAT3& v)
		{
			DirectX::XMVECTOR rotationVec = DirectX::XMQuaternionRotationRollPitchYaw(Rotation.x,
				Rotation.y,
				Rotation.z);
			auto rotationMat = DirectX::XMMatrixRotationQuaternion(rotationVec);

			DirectX::XMVECTOR vVec = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(v.x, v.y, v.z, 0), rotationMat);
			DirectX::XMStoreFloat3(&v, vVec);
		}
	};

	struct MeshRendererComponent final
	{
		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;
		MeshRendererComponent(const VertexBufferData& vertexBufferData, const IndexBufferData& indexBufferData, const std::string& shaderFilePath)
		{
			pVertexBuffer.reset(VertexBuffer::Create(vertexBufferData));
			pIndexBuffer.reset(IndexBuffer::Create(indexBufferData));
			pShader = Shader::Create(shaderFilePath, vertexBufferData.BufferLayout);
		}

		Ref<VertexBuffer> pVertexBuffer = nullptr;
		Ref<IndexBuffer> pIndexBuffer = nullptr;
		Ref<Shader> pShader = nullptr;
	};

	struct StaticMeshComponent final
	{
		StaticMeshComponent() = default;
		StaticMeshComponent(const StaticMeshComponent&) = default;

		StaticMeshComponent(const std::string& assetFile)
			: StaticMeshComponent(assetFile, BufferLayout{
				{ ShaderDataType::eFloat3, "Position" },
				{ ShaderDataType::eFloat3, "Normal" },
				{ ShaderDataType::eFloat2, "TexCoord" },
				{ ShaderDataType::eFloat3, "Tangent" }
				})
		{}

		StaticMeshComponent(const std::string& assetFile, const BufferLayout& layout)
		{
			MeshLoader meshLoader{};
			m_pMeshes = meshLoader.LoadMesh(assetFile);

			for (const auto& pMesh : m_pMeshes)
			{
				pMesh->Create(layout);
			}
		}

		std::vector<Ref<Mesh>> m_pMeshes = {};
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
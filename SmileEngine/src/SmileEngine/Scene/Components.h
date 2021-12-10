#pragma once
#include <DirectXMath.h>

#include "SmileEngine/Renderer/Buffer.h"
#include "SmileEngine/Renderer/Shader.h"

#include "SmileEngine/Scene/SceneCamera.h"

#include "SmileEngine/Core/MeshLoader.h"
#include "SmileEngine/Renderer/Mesh.h"
#include "SmileEngine/Renderer/Material.h"
#include "SmileEngine/Renderer/Animation/MeshAnimator.h"

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
			DirectX::XMFLOAT3 forward{ 0, 0, 1 };
			RotateVector(forward);
			return forward;
		}

		DirectX::XMFLOAT3 GetRight()
		{
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
		StaticMeshComponent()
		{
			pMaterials.push_back(CreateRef<Material>());
		}

		StaticMeshComponent(const StaticMeshComponent&) = default;

		// For now, only support 1 material
		StaticMeshComponent(const std::string& assetFile, const Ref<Material>& pMaterial)
		{
			pMaterials.push_back(pMaterial);

			MeshLoader meshLoader{};
			pMeshes = meshLoader.LoadMesh(assetFile);

			for (const auto& pMesh : pMeshes)
			{
				const auto& bufferLayout = pMaterials[0]->GetBufferLayout();
				pMesh->Create(bufferLayout);

				MeshAnimator animator{ pMesh };
				/*animator.SetAnimation(0);
				animator.Play();*/
				Animators.push_back(animator);
			}
		}

		std::vector<Ref<Mesh>> pMeshes = {};
		std::vector<Ref<Material>> pMaterials = {};
		std::vector<MeshAnimator> Animators = {};
	};

	struct CameraComponent final
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		SceneCamera Camera;
		bool bPrimary = true;
		bool bFixedAspectRatio = false;
	};
}
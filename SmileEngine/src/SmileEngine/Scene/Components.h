#pragma once
#include "SmileEngine/Core/UUID.h"
#include "SmileEngine/Renderer/Buffer.h"
#include "SmileEngine/Renderer/Shader.h"

#include "SmileEngine/Scene/SceneCamera.h"

#include "SmileEngine/Core/MeshLoader.h"
#include "SmileEngine/Renderer/StaticMeshFilter.h"
#include "SmileEngine/Renderer/SkinnedMeshFilter.h"
#include "SmileEngine/Renderer/Material.h"
#include "SmileEngine/Renderer/Animation/MeshAnimator.h"

#include "SmileEngine/Physics/PhysicsMaterial.h"

#include <DirectXMath.h>

namespace Smile
{
	struct IDComponent
	{
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

		UUID ID;
	};

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

			pMeshes = MeshLoader::LoadStaticMesh(assetFile);
			const auto& bufferLayout = pMaterials[0]->GetBufferLayout();
			for (const auto& pMesh : pMeshes)
			{
				pMesh->Create(bufferLayout);
			}
		}

		std::vector<Ref<StaticMeshFilter>> pMeshes = {};
		std::vector<Ref<Material>> pMaterials = {};
	};

	struct SkinnedMeshComponent final
	{
		SkinnedMeshComponent()
		{
			pMaterials.push_back(CreateRef<Material>(true));
		}

		SkinnedMeshComponent(const SkinnedMeshComponent&) = default;

		// For now, only support 1 material
		SkinnedMeshComponent(const std::string& assetFile, const Ref<Material>& pMaterial)
		{
			pMaterials.push_back(pMaterial);

			pMeshes = MeshLoader::LoadSkinnedMesh(assetFile);
			const auto& bufferLayout = pMaterials[0]->GetBufferLayout();
			for (const auto& pMesh : pMeshes)
			{
				pMesh->Create(bufferLayout);

				if (pMesh->HasAnimations())
				{
					MeshAnimator animator{ pMesh };
					Animators.push_back(animator);
				}
			}
		}

		std::vector<Ref<SkinnedMeshFilter>> pMeshes = {};
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

	struct RigidbodyComponent final
	{
		enum class BodyType { eStatic = 0, eDynamic = 1 };
		enum class CollisionDetectionType { eDiscrete = 0, eContinuous = 1 };

		RigidbodyComponent() = default;
		RigidbodyComponent(const RigidbodyComponent&) = default;

		// Data
		BodyType Type;
		CollisionDetectionType CollisionDetection = CollisionDetectionType::eDiscrete;
		Ref<PhysicsMaterial> pPhysicsMaterial = nullptr;

		float Mass = 1.0f;
		float LinearDrag = 0.0f;
		float AngularDrag = 0.05f;

		bool bDisableGravity = false;
		bool bKinematic = false;

		bool bLockPositionX = false;
		bool bLockPositionY = false;
		bool bLockPositionZ = false;
		bool bLockRotationX = false;
		bool bLockRotationY = false;
		bool bLockRotationZ = false;
	};

	struct BoxColliderComponent final
	{
		BoxColliderComponent() = default;
		BoxColliderComponent(const BoxColliderComponent&) = default;

		DirectX::XMFLOAT3 Size = { 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 Offset = { 0.0f, 0.0f, 0.0f };
		bool bTrigger = false;
		bool bShowColliderBounds = true;

		Ref<PhysicsMaterial> pPhysicsMaterial = nullptr;
	};

	struct SphereColliderComponent final
	{
		SphereColliderComponent() = default;
		SphereColliderComponent(const SphereColliderComponent&) = default;

		float Radius = 0.5f;
		bool bTrigger = false;
		bool bShowColliderBounds = true;
		
		Ref<PhysicsMaterial> pPhysicsMaterial = nullptr;
	};

	struct CapsuleColliderComponent final
	{
		CapsuleColliderComponent() = default;
		CapsuleColliderComponent(const CapsuleColliderComponent&) = default;

		float Radius = 0.5f;
		float Height = 1.0f;
		bool bTrigger = false;
		bool bShowColliderBounds = true;

		Ref<PhysicsMaterial> pPhysicsMaterial = nullptr;
	};
}
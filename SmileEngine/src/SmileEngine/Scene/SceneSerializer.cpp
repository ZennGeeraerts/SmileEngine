#include "smpch.h"
#include "SceneSerializer.h"

#include "SmileEngine/Core/Logger.h"
#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace YAML
{
	template<>
	struct convert<DirectX::XMFLOAT3>
	{
		static Node encode(const DirectX::XMFLOAT3& v)
		{
			Node node{};
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			return node;
		}

		static bool decode(const Node& node, DirectX::XMFLOAT3& v)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<DirectX::XMFLOAT4>
	{
		static Node encode(const DirectX::XMFLOAT4& v)
		{
			Node node{};
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.push_back(v.w);
			return node;
		}

		static bool decode(const Node& node, DirectX::XMFLOAT4& v)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			v.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Smile
{
	YAML::Emitter& operator<<(YAML::Emitter& output, const DirectX::XMFLOAT3& v)
	{
		output << YAML::Flow;
		output << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return output;
	}

	YAML::Emitter& operator<<(YAML::Emitter& output, const DirectX::XMFLOAT4& v)
	{
		output << YAML::Flow;
		output << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return output;
	}



	SceneSerializer::SceneSerializer(const Ref<Scene>& pScene)
		: m_pScene{ pScene }
	{

	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter output{};
		output << YAML::BeginMap;
		output << YAML::Key << "Scene" << YAML::Value << "Untitled";
		output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_pScene->m_Registry.each([&] (auto entityID)
			{
				Entity entity{ entityID, m_pScene.get() };
				if (!entity)
					return;
				SerializeEntity(output, entity);
			});

		output << YAML::EndSeq;
		output << YAML::EndMap;

		std::ofstream fileOutput{ filePath };
		fileOutput << output.c_str();
	}

	static void SerializeEntity(YAML::Emitter& output, Entity entity)
	{
		output << YAML::BeginMap;
		output << YAML::Key << "Entity" << YAML::Value << "1283719283174164";

		if (entity.HasComponent<TagComponent>())
		{
			output << YAML::Key << "TagComponent";
			output << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			output << YAML::Key << "Tag" << YAML::Value << tag;

			output << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			output << YAML::Key << "TransformComponent";
			output << YAML::BeginMap;

			auto& transformComponent = entity.GetComponent<TransformComponent>();
			output << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
			output << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
			output << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;

			output << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			output << YAML::Key << "CameraComponent";
			output << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;
			output << YAML::Key << "Camera" << YAML::Value;
			output << YAML::BeginMap;

			output << YAML::Key << "ProjectionType" << YAML::Value << static_cast<uint32_t>(camera.GetProjectionType());
			output << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
			output << YAML::Key << "PerspectiveNearPlane" << YAML::Value << camera.GetPerspectiveNearPlane();
			output << YAML::Key << "PerspectiveFarPlane" << YAML::Value << camera.GetPerspectiveFarPlane();
			output << YAML::Key << "Size" << YAML::Value << camera.GetSize();
			output << YAML::Key << "OrthographicNearPlane" << camera.GetOrthographicNearPlane();
			output << YAML::Key << "OrthographicFarPlane" << camera.GetOrthographicFarPlane();

			output << YAML::EndMap;

			output << YAML::Key << "bPrimary" << YAML::Value << cameraComponent.bPrimary;
			output << YAML::Key << "bFixedAspectRatio" << YAML::Value << cameraComponent.bFixedAspectRatio;

			output << YAML::EndMap;
		}

		if (entity.HasComponent<MeshComponent>())
		{
			output << YAML::Key << "MeshComponent";
			output << YAML::BeginMap;

			auto& meshComponent = entity.GetComponent<MeshComponent>();
			output << YAML::Key << "Mesh" << YAML::Value << ((meshComponent.pMeshes.size() > 0) ? meshComponent.pMeshes[0]->GetFilePath() : "");

			output << YAML::Key << "Material";
			output << YAML::BeginMap;

			auto& pMaterial = meshComponent.pMaterials[0];
			output << YAML::Key << "AlbedoMap" << YAML::Value <<  (pMaterial->GetAlbedoMap() ? pMaterial->GetAlbedoMap()->GetFilePath() : "");
			output << YAML::Key << "AlbedoColor" << YAML::Value << pMaterial->GetAlbedoColor();

			output << YAML::Key << "MetalnessMap" << YAML::Value << (pMaterial->GetMetalnessMap() ? pMaterial->GetMetalnessMap()->GetFilePath() : "");
			output << YAML::Key << "Metalness" << YAML::Value << pMaterial->GetMetalness();

			output << YAML::Key << "RoughnessMap" << YAML::Value << (pMaterial->GetRoughnessMap() ? pMaterial->GetRoughnessMap()->GetFilePath() : "");
			output << YAML::Key << "Roughness" << YAML::Value << pMaterial->GetRoughness();

			output << YAML::Key << "NormalMap" << YAML::Value << (pMaterial->GetNormalMap() ? pMaterial->GetNormalMap()->GetFilePath() : "");
			output << YAML::Key << "AOMap" << YAML::Value << (pMaterial->GetAOMap() ? pMaterial->GetAOMap()->GetFilePath() : "");

			output << YAML::EndMap;

			output << YAML::EndMap;
		}

		output << YAML::EndMap;
	}

	void SceneSerializer::SerializeRuntime(const std::string& filePath)
	{
		SM_ASSERT(false, "SceneSerializer::SerializeRuntime > Not implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& filePath)
	{
		std::ifstream fileInput{ filePath };
		std::stringstream strStream{};
		strStream << fileInput.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		SM_LOG_TRACE("Deserializing scene '%s'", sceneName.c_str());

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name{};
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				SM_LOG_TRACE("Deserialized entity with ID: %d, name: %s", uuid, name.c_str());

				Entity deserializedEntity = m_pScene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<DirectX::XMFLOAT3>();
					tc.Rotation = transformComponent["Rotation"].as<DirectX::XMFLOAT3>();
					tc.Scale = transformComponent["Scale"].as<DirectX::XMFLOAT3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(cameraProps["ProjectionType"].as<int>()));

					cc.Camera.SetFOV(cameraProps["FOV"].as<float>());
					cc.Camera.SetPerspectiveNearPlane(cameraProps["PerspectiveNearPlane"].as<float>());
					cc.Camera.SetPerspectiveFarPlane(cameraProps["PerspectiveFarPlane"].as<float>());

					cc.Camera.SetSize(cameraProps["Size"].as<float>());
					cc.Camera.SetOrthographicNearPlane(cameraProps["OrthographicNearPlane"].as<float>());
					cc.Camera.SetOrthographicFarPlane(cameraProps["OrthographicFarPlane"].as<float>());

					cc.bPrimary = cameraComponent["bPrimary"].as<bool>();
					cc.bFixedAspectRatio = cameraComponent["bFixedAspectRatio"].as<bool>();
				}

				auto meshComponent = entity["MeshComponent"];
				if (meshComponent)
				{
					auto& mc = deserializedEntity.AddComponent<MeshComponent>();

					const auto& meshPath = meshComponent["Mesh"].as<std::string>();
					if (!meshPath.empty())
					{
						MeshLoader meshLoader{};
						mc.pMeshes = meshLoader.LoadMesh(meshPath);

						const auto& bufferLayout = mc.pMaterials[0]->GetBufferLayout();
						for (const auto& pMesh : mc.pMeshes)
						{
							pMesh->Create(bufferLayout);

							if (pMesh->HasAnimations())
							{
								MeshAnimator animator{ pMesh };
								mc.Animators.push_back(animator);
								mc.Animators.back().SetAnimation(0);
							}
						}
					}

					auto material = meshComponent["Material"];

					const auto& albedoMap = material["AlbedoMap"].as<std::string>();
					if (!albedoMap.empty())
					{
						mc.pMaterials[0]->SetAlbedo(Texture2D::Create(albedoMap));
						mc.pMaterials[0]->SetUseAlbedoMap(true);
					}
					mc.pMaterials[0]->SetAlbedo(material["AlbedoColor"].as<DirectX::XMFLOAT3>());

					const auto& metalnessMap = material["MetalnessMap"].as<std::string>();
					if (!metalnessMap.empty())
					{
						mc.pMaterials[0]->SetMetalness(Texture2D::Create(metalnessMap));
						mc.pMaterials[0]->SetUseMetalnessMap(true);
					}
					mc.pMaterials[0]->SetMetalness(material["Metalness"].as<float>());

					const auto& roughnessMap = material["RoughnessMap"].as<std::string>();
					if (!roughnessMap.empty())
					{
						mc.pMaterials[0]->SetRoughness(Texture2D::Create(roughnessMap));
						mc.pMaterials[0]->SetUseRoughnessMap(true);
					}
					mc.pMaterials[0]->SetRoughness(material["Roughness"].as<float>());

					const auto& normalMap = material["NormalMap"].as<std::string>();
					if (!normalMap.empty())
					{
						mc.pMaterials[0]->SetNormalMap(Texture2D::Create(normalMap));
						mc.pMaterials[0]->SetUseNormalMap(true);
					}

					const auto& aoMap = material["AOMap"].as<std::string>();
					if (!aoMap.empty())
					{
						mc.pMaterials[0]->SetAOMap(Texture2D::Create(aoMap));
						mc.pMaterials[0]->SetUseAOMap(true);
					}
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filePath)
	{
		SM_ASSERT(false, "SceneSerializer::DeserializeRuntime > Not implemented");
		return false;
	}
}
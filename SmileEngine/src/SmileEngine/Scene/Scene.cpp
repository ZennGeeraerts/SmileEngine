#include "smpch.h"
#include "Scene.h"

#include "Components.h"
#include "SmileEngine/Renderer/Renderer.h"
#include "SmileEngine/Core/SmileGame.h"

#include "Entity.h"

namespace Smile
{
	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);

		return entity;
	}

	Entity Scene::CreateEntity() 
	{ 
		return CreateEntity("Entity"); 
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateRuntime(Timestep deltaTime)
	{
		Camera* pMainCamera = nullptr;
		DirectX::XMFLOAT4X4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				const auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.bPrimary)
				{
					pMainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (pMainCamera)
		{
			Renderer::BeginScene(*pMainCamera, cameraTransform);

			{
				auto group = m_Registry.group<MeshRendererComponent>(entt::get<TransformComponent>);
				for (auto entity : group)
				{
					const auto& [mesh, transform] = group.get<MeshRendererComponent, TransformComponent>(entity);
					Renderer::Submit(mesh, transform.GetTransform());
				}
			}
			{
				auto group = m_Registry.group<StaticMeshComponent>(entt::get<TransformComponent>);
				for (auto entity : group)
				{
					const auto& [mesh, transform] = group.get<StaticMeshComponent, TransformComponent>(entity);
					Renderer::Submit(mesh, transform.GetTransform());
				}
			}
			{
				auto group = m_Registry.group<SkinnedMeshComponent>(entt::get<TransformComponent>);
				for (auto entity : group)
				{
					const auto& [mesh, transform] = group.get<SkinnedMeshComponent, TransformComponent>(entity);

					for (auto& animator : mesh.Animators)
					{
						animator.OnUpdate(deltaTime);
						const auto& boneTransforms = animator.GetBoneTransforms();
						for (const auto& pMaterial : mesh.pMaterials)
						{
							if (animator.IsPlaying())
								pMaterial->GetShader()->UploadMat4Array("Bones", boneTransforms);
						}
					}

					Renderer::Submit(mesh, transform.GetTransform());
				}
			}

			Renderer::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep deltaTime, EditorCamera& editorCamera)
	{
		Renderer::BeginScene(editorCamera);

		{
			auto group = m_Registry.group<MeshRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				const auto& [mesh, transform] = group.get<MeshRendererComponent, TransformComponent>(entity);
				Renderer::Submit(mesh, transform.GetTransform());
			}
		}
		{
			auto group = m_Registry.group<StaticMeshComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				const auto& [mesh, transform] = group.get<StaticMeshComponent, TransformComponent>(entity);
				Renderer::Submit(mesh, transform.GetTransform());
			}
		}
		{
			auto group = m_Registry.group<SkinnedMeshComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				const auto& [mesh, transform] = group.get<SkinnedMeshComponent, TransformComponent>(entity);

				for (auto& animator : mesh.Animators)
				{
					animator.OnUpdate(deltaTime);
					const auto& boneTransforms = animator.GetBoneTransforms();
					for (const auto& pMaterial : mesh.pMaterials)
					{
						if (animator.IsPlaying())
							pMaterial->GetShader()->UploadMat4Array("Bones", boneTransforms);
					}
				}

				Renderer::Submit(mesh, transform.GetTransform());
			}
		}

		Renderer::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.bFixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (cameraComponent.bPrimary)
				return Entity{ entity, this };
		}
		return Entity{};
	}

	template <typename ComponentType>
	void Scene::OnComponentAdded(Entity entity, ComponentType& component)
	{
		static_assert(false);
	}

	template <>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template <>
	void Scene::OnComponentAdded<MeshRendererComponent>(Entity entity, MeshRendererComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded<StaticMeshComponent>(Entity entity, StaticMeshComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded<SkinnedMeshComponent>(Entity entity, SkinnedMeshComponent& component)
	{
	}
}

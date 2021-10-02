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

	void Scene::OnUpdate(Timestep deltaTime)
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

			Renderer::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
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
}

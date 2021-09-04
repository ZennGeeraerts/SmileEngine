#include "smpch.h"
#include "Scene.h"

#include "Components.h"
#include "SmileEngine/Renderer/Renderer.h"

#include "Entity.h"

namespace Smile
{
	Scene::Scene()
	{
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity);
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
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
		auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
		for (auto entity : group)
		{
			const auto& [transform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);
		}
	}
}

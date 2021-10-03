#pragma once

#include "SmileEngine/Core/Timestep.h"

#include "Libs/entt/entt.hpp"

namespace Smile
{
	class Entity;

	class Scene final
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name);
		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep deltaTime);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template <typename ComponentType>
		void OnComponentAdded(Entity entity, ComponentType& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}


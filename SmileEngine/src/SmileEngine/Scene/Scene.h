#pragma once

#include "SmileEngine/Core/UUID.h"
#include "SmileEngine/Core/Timestep.h"
#include "SmileEngine/Renderer/EditorCamera.h"

#include <Libs/entt/entt.hpp>

namespace Smile
{
	class Entity;

	class Scene final
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);
		Entity CreateEntity(UUID uuid, const std::string& name);
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep deltaTime);
		void OnUpdateEditor(Timestep deltaTime, EditorCamera& editorCamera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

	private:
		template <typename ComponentType>
		void OnComponentAdded(Entity entity, ComponentType& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}


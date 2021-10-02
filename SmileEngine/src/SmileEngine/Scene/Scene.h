#pragma once

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

		void OnUpdate(Timestep deltaTime);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}


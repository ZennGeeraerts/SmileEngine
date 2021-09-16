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

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}


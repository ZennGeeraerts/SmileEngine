#pragma once

#include "entt/entt.hpp"

namespace Smile
{
	class Entity;

	class SMILE_API Scene final
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


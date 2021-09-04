#pragma once
#include "smpch.h"

namespace Smile
{
	class SMILE_API Entity final
	{
	public:
		Entity(entt::entity handle, Scene* pScene)
			: m_EntityHandle{ handle }
			, m_pScene{ pScene }
		{
		}

		Entity(const Entity&) = default;

		template <typename ComponentType, typename... ConstructorArgs>
		ComponentType& AddComponent(ConstructorArgs&&... constructorArgs)
		{
			SM_ASSERT(!HasComponent<ComponentType>(), "Entity::AddComponent > Entity already has component");

			// forward the constructor arguments to entt
			return m_pScene->m_Registry.emplace<ComponentType>(m_EntityHandle, std::forward<ConstructorArgs>(constructorArgs)...);
		}

		template <typename ComponentType>
		void RemoveComponent()
		{
			m_pScene->m_Registry.remove<ComponentType>(m_EntityHandle);
		}

		template <typename ComponentType>
		ComponentType& GetComponent() const
		{
			SM_ASSERT(HasComponent<ComponentType>(), "Entity::AddComponent > Entity does not have component");

			return m_pScene->m_Registry.get<ComponentType>(m_EntityHandle);
		}

		template <typename ComponentType>
		bool HasComponent() const
		{
			return m_pScene->m_Registry.all_of<ComponentType>(m_EntityHandle);
		}

		// Check to see if entity is valid
		operator bool() const { return m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_pScene = nullptr;
	};
}


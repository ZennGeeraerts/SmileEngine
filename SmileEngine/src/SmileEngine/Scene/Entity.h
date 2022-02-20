#pragma once

#include "SmileEngine/Core/UUID.h"
#include "Scene.h"
#include "Components.h"

#include <Libs/entt/entt.hpp>

namespace smile
{
    class Entity final
    {
      public:
        Entity() = default;

        Entity( entt::entity handle, Scene *pScene ) : m_EntityHandle{ handle }, m_pScene{ pScene }
        {
        }

        Entity( const Entity & ) = default;

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddComponent( ConstructorArgs &&...constructorArgs )
        {
            SM_ASSERT( !HasComponent< ComponentType >(), "Entity::AddComponent > Entity already has component" );

            // forward the constructor arguments to entt
            ComponentType &component = m_pScene->m_Registry.emplace< ComponentType >(
                m_EntityHandle, std::forward< ConstructorArgs >( constructorArgs )... );
            m_pScene->OnComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddOrReplaceComponent( ConstructorArgs &&...constructorArgs )
        {
            // forward the constructor arguments to entt
            ComponentType &component = m_pScene->m_Registry.emplace_or_replace< ComponentType >(
                m_EntityHandle, std::forward< ConstructorArgs >( constructorArgs )... );
            m_pScene->OnComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType >
        void RemoveComponent()
        {
            m_pScene->m_Registry.remove< ComponentType >( m_EntityHandle );
        }

        template < typename ComponentType >
        ComponentType &GetComponent() const
        {
            SM_ASSERT( HasComponent< ComponentType >(), "Entity::GetComponent > Entity does not have component" );

            return m_pScene->m_Registry.get< ComponentType >( m_EntityHandle );
        }

        UUID GetUUID() const
        {
            return GetComponent< IDComponent >().m_ID;
        }
        const std::string &GetName() const
        {
            return GetComponent< TagComponent >().m_Tag;
        }
        DirectX::XMFLOAT4X4 GetTransform() const
        {
            return GetComponent< TransformComponent >().GetTransform();
        }

        template < typename ComponentType >
        bool HasComponent() const
        {
            return m_pScene->m_Registry.all_of< ComponentType >( m_EntityHandle );
        }

        // Check to see if entity is valid
        operator bool() const
        {
            return m_EntityHandle != entt::null;
        }
        operator entt::entity() const
        {
            return m_EntityHandle;
        }
        operator uint32_t() const
        {
            return static_cast< uint32_t >( m_EntityHandle );
        }

        bool operator==( Entity other ) const
        {
            return ( m_EntityHandle == other.m_EntityHandle ) && ( m_pScene == other.m_pScene );
        }
        bool operator!=( Entity other ) const
        {
            return !( *this == other );
        }

      private:
        entt::entity m_EntityHandle = entt::null;
        Scene *m_pScene = nullptr;
    };
}

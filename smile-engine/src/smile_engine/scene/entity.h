#pragma once

#include "smile_engine/core/uuid.h"
#include "scene.h"
#include "components.h"

#include "smile_engine/ecs/ecs_engine.h"

namespace Smile::Scene
{
    class Entity final
    {
      public:
        Entity() = default;

        Entity( ECS::EntityHandleType handle, Scene *pScene ) : m_EntityHandle{ handle }, m_pScene{ pScene }
        {
        }

        Entity( const Entity & ) = default;

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddComponent( ConstructorArgs &&...constructor_args )
        {
            SM_ASSERT( !HasComponent< ComponentType >(), "Entity::AddComponent > Entity already has component" );

            // forward the constructor arguments
            ComponentType &component = m_pScene->m_ECSEngine.AddComponent< ComponentType >(
                m_EntityHandle, std::forward< ConstructorArgs >( constructor_args )... );
            m_pScene->OnComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddOrReplaceComponent( ConstructorArgs &&...constructorArgs )
        {
            // forward the constructor arguments
            ComponentType &component = m_pScene->m_ECSEngine.AddOrReplaceComponent< ComponentType >(
                m_EntityHandle, std::forward< ConstructorArgs >( constructorArgs )... );
            m_pScene->OnComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType >
        void RemoveComponent()
        {
            m_pScene->m_ECSEngine.RemoveComponent< ComponentType >( m_EntityHandle );
        }

        template < typename ComponentType >
        ComponentType &GetComponent() const
        {
            SM_ASSERT( HasComponent< ComponentType >(), "Entity::GetComponent > Entity does not have component" );

            return m_pScene->m_ECSEngine.GetComponent< ComponentType >( m_EntityHandle );
        }

        UUID GetUUID() const
        {
            return GetComponent< IDComponent >().ID;
        }
        const std::string &GetName() const
        {
            return GetComponent< TagComponent >().Tag;
        }
        DirectX::XMFLOAT4X4 GetTransform() const
        {
            return GetComponent< TransformComponent >().getTransform();
        }

        template < typename ComponentType >
        bool HasComponent() const
        {
            return m_pScene->m_ECSEngine.HasComponent< ComponentType >( m_EntityHandle );
        }

        // Check to see if entity is valid
        operator bool() const
        {
            return m_EntityHandle != ECS::g_NullHandle< Uint32 >;
        }
        operator ECS::EntityHandleType() const
        {
            return m_EntityHandle;
        }
        operator Uint32() const
        {
            return static_cast< Uint32 >( m_EntityHandle.Hash() );
        }
        operator Uint64() const
        {
            return m_EntityHandle.Hash();
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
        ECS::EntityHandleType m_EntityHandle = ECS::g_NullHandle< Uint32 >;
        Scene *m_pScene = nullptr;
    };
}

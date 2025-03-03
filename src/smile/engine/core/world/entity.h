/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "primitive/uuid.h"
#include "world.h"
#include "ecs/id_component.h"
#include "ecs/tag_component.h"
#include "ecs/transform_component.h"

#include "engine/core/ecs/ecs_engine.h"

namespace smile::world
{
    class Entity final
    {
      public:
        Entity() = default;
        Entity( smile::ecs::EntityHandle handle, World *pWorld );
        Entity( const Entity & ) = default;

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddComponent( ConstructorArgs &&...constructor_args )
        {
            SM_ASSERT( !HasComponent< ComponentType >(), "Entity::AddComponent > Entity already has component" );

            // forward the constructor arguments
            ComponentType &component = m_pWorld->m_ECSEngine.AddComponent< ComponentType >(
                m_EntityHandle, std::forward< ConstructorArgs >( constructor_args )... );
            m_pWorld->OnComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddOrReplaceComponent( ConstructorArgs &&...constructorArgs )
        {
            // forward the constructor arguments
            ComponentType &component = m_pWorld->m_ECSEngine.AddOrReplaceComponent< ComponentType >(
                m_EntityHandle, std::forward< ConstructorArgs >( constructorArgs )... );
            m_pWorld->OnComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType >
        void RemoveComponent()
        {
            m_pWorld->m_ECSEngine.RemoveComponent< ComponentType >( m_EntityHandle );
        }

        template < typename ComponentType >
        ComponentType &GetComponent() const
        {
            SM_ASSERT( HasComponent< ComponentType >(), "Entity::GetComponent > Entity does not have component" );

            return m_pWorld->m_ECSEngine.GetComponent< ComponentType >( m_EntityHandle );
        }

        template < typename ComponentType >
        ComponentType *TryGetComponent() const
        {
            return m_pWorld->m_ECSEngine.TryGetComponent< ComponentType >( m_EntityHandle );
        }

        primitive::UUID GetUUID() const
        {
            return GetComponent< ecs::IDComponent >().ID;
        }
        const std::string &GetName() const
        {
            return GetComponent< ecs::TagComponent >().Tag;
        }
        DirectX::XMFLOAT4X4 GetTransform() const
        {
            return GetComponent< ecs::TransformComponent >().GetWorldTransform();
        }

        template < typename ComponentType >
        bool HasComponent() const
        {
            return m_pWorld->m_ECSEngine.HasComponent< ComponentType >( m_EntityHandle );
        }

        void AddChild( Entity child );
        void RemoveChild( Entity child );

        Uint32 GetChildrenCount() const;
        Entity GetChildAtIndex( const Uint32 index ) const;

        Entity GetParent() const;

        // Check to see if entity is valid
        operator bool() const
        {
            return m_EntityHandle != smile::ecs::EntityHandle::NullHandle();
        }
        operator smile::ecs::EntityHandle() const
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
            return ( m_EntityHandle == other.m_EntityHandle ) && ( m_pWorld == other.m_pWorld );
        }
        bool operator!=( Entity other ) const
        {
            return !( *this == other );
        }

      private:
        smile::ecs::EntityHandle m_EntityHandle = smile::ecs::EntityHandle::NullHandle();
        World *m_pWorld = nullptr;
    };
}
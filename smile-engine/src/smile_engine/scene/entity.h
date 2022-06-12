#pragma once

#include "smile_engine/core/uuid.h"
#include "scene.h"
#include "components.h"

#include <thirdparty/entt/entt.hpp>

namespace smile::scene
{
    class Entity final
    {
      public:
        Entity() = default;

        Entity( entt::entity handle, Scene *scene ) : entityHandle{ handle }, scene{ scene }
        {
        }

        Entity( const Entity & ) = default;

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &addComponent( ConstructorArgs &&...constructor_args )
        {
            SM_ASSERT( !hasComponent< ComponentType >(), "Entity::addComponent > Entity already has component" );

            // forward the constructor arguments to entt
            ComponentType &component = scene->registry.emplace< ComponentType >(
                entityHandle, std::forward< ConstructorArgs >( constructor_args )... );
            scene->onComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &addOrReplaceComponent( ConstructorArgs &&...constructor_args )
        {
            // forward the constructor arguments to entt
            ComponentType &component = scene->registry.emplace_or_replace< ComponentType >(
                entityHandle, std::forward< ConstructorArgs >( constructor_args )... );
            scene->onComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType >
        void removeComponent()
        {
            scene->registry.remove< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        ComponentType &getComponent() const
        {
            SM_ASSERT( hasComponent< ComponentType >(), "Entity::getComponent > Entity does not have component" );

            return scene->registry.get< ComponentType >( entityHandle );
        }

        UUID getUUID() const
        {
            return getComponent< IDComponent >().id;
        }
        const std::string &getName() const
        {
            return getComponent< TagComponent >().tag;
        }
        DirectX::XMFLOAT4X4 getTransform() const
        {
            return getComponent< TransformComponent >().getTransform();
        }

        template < typename ComponentType >
        bool hasComponent() const
        {
            return scene->registry.all_of< ComponentType >( entityHandle );
        }

        // Check to see if entity is valid
        operator bool() const
        {
            return entityHandle != entt::null;
        }
        operator entt::entity() const
        {
            return entityHandle;
        }
        operator Uint32() const
        {
            return static_cast< Uint32 >( entityHandle );
        }

        bool operator==( Entity other ) const
        {
            return ( entityHandle == other.entityHandle ) && ( scene == other.scene );
        }
        bool operator!=( Entity other ) const
        {
            return !( *this == other );
        }

      private:
        entt::entity entityHandle = entt::null;
        Scene *scene = nullptr;
    };
}

#pragma once

#include "smile_engine/core/uuid.h"
#include "scene.h"
#include "components.h"

#include "smile_engine/ecs/ecs_engine.h"
//#include <thirdparty/entt/entt.hpp>

namespace smile::scene
{
    class Entity final
    {
      public:
        Entity() = default;

        Entity( ecs::EntityHandleType handle, Scene *scene ) : entityHandle{ handle }, scene{ scene }
        {
        }

        Entity( const Entity & ) = default;

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &addComponent( ConstructorArgs &&...constructor_args )
        {
            SM_ASSERT( !hasComponent< ComponentType >(), "Entity::addComponent > Entity already has component" );

            // forward the constructor arguments to entt
            ComponentType &component = scene->ecsEngine.addComponent< ComponentType >(
                entityHandle, std::forward< ConstructorArgs >( constructor_args )... );
            scene->onComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &addOrReplaceComponent( ConstructorArgs &&...constructor_args )
        {
            // forward the constructor arguments to entt
            ComponentType &component = scene->ecsEngine.addOrReplaceComponent< ComponentType >(
                entityHandle, std::forward< ConstructorArgs >( constructor_args )... );
            scene->onComponentAdded< ComponentType >( *this, component );
            return component;
        }

        template < typename ComponentType >
        void removeComponent()
        {
            scene->ecsEngine.removeComponent< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        ComponentType &getComponent() const
        {
            SM_ASSERT( hasComponent< ComponentType >(), "Entity::getComponent > Entity does not have component" );

            return scene->ecsEngine.getComponent< ComponentType >( entityHandle );
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
            return scene->ecsEngine.hasComponent< ComponentType >( entityHandle );
        }

        // Check to see if entity is valid
        operator bool() const
        {
            return entityHandle != ecs::nullHandle< Uint32 >;
        }
        operator ecs::EntityHandleType() const
        {
            return entityHandle;
        }
        operator Uint32() const
        {
            return static_cast< Uint32 >( entityHandle.hash() );
        }
        operator Uint64() const
        {
            return entityHandle.hash();
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
        //entt::entity entityHandle = entt::null;
        ecs::EntityHandleType entityHandle = ecs::nullHandle< Uint32 >;
        Scene *scene = nullptr;
    };
}

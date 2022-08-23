#pragma once

#include "entity_handle_manager.h"
#include "component_interface.h"

#include "smile_engine/core/compiled/type_id.h"

namespace smile::ecs
{
    class ECSEngine final
    {
      public:
        ECSEngine() = default;
        virtual ~ECSEngine();

        EntityHandle createEntity()
        {
            return handleManager.createEntity();
        }

        void destroyEntity( EntityHandle entity_handle )
        {
            handleManager.destroyEntity( entity_handle );

            for ( auto component_interface : components )
            {
                removeComponent( component_interface, entity_handle );
            }
        }

        bool isEntityActive( EntityHandle entity_handle ) const
        {
            return handleManager.isEntityActive( entity_handle );
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &addComponent( EntityHandle entity_handle, ConstructorArgs &&...constructor_args )
        {
            registerComponentIfNeeded< ComponentType >();

            ComponentInterface *component_interface = getComponentInterface< ComponentType >();
            ComponentStorage *component_storage = getComponentStorage< ComponentType >();

            const auto index = component_interface->sparseSet.insert( entity_handle.index );

            SM_ASSERT( index == component_storage->getSize(),
                "ECSEngine::addComponent > Failed to add component" );

            return component_storage->append< ComponentType >(
                entity_handle.index, std::forward< ConstructorArgs >( constructor_args )... );
        }

        template < typename ComponentType >
        void removeComponent( EntityHandle entity_handle )
        {
            ComponentInterface *component_interface = getComponentInterface< ComponentType >();
            removeComponent( component_interface, entity_handle );
        }

        void removeComponent( ComponentInterface *component_interface, EntityHandle entity_handle );

        template < typename ComponentType >
        ComponentType &getComponent( EntityHandle entity_handle )
        {
            auto it = componentMap.find( typeIDOf< ComponentType >() );

            SM_ASSERT( it != componentMap.end(), "ECSEngine::getComponent > Component is missing" );

            return it->second->get< ComponentType >( entity_handle );
        }

        template < typename ComponentType >
        const ComponentType &getComponent( EntityHandle entity_handle ) const
        {
            auto it = componentMap.find( typeIDOf< ComponentType >() );

            SM_ASSERT( it != componentMap.end(), "ECSEngine::getComponent > Component is missing" );

            return it->second->get< ComponentType >( entity_handle );
        }

      private:
        template < typename ComponentType >
        void registerComponent( bool relational = false )
        {
            ComponentInterface *component_interface = new ComponentInterface{};
            component_interface->relational = relational;
            component_interface->componentStorage = new ComponentStorageHandler< ComponentType >( relational );

            components.push_back( component_interface );

            auto type_id = typeIDOf< ComponentType >();
            componentMap[type_id] = component_interface;
        }

        template < typename ComponentType >
        void registerComponentIfNeeded( bool relational = false )
        {
            auto type_id = typeIDOf< ComponentType >();
            if ( componentMap.find( type_id ) == componentMap.end() )
                registerComponent< ComponentType >( relational );
        }

        template < typename ComponentType >
        ComponentInterface *getComponentInterface()
        {
            auto type_id = typeIDOf< ComponentType >();
            return componentMap[type_id];
        }

        template < typename ComponentType >
        const ComponentInterface *getComponentInterface() const
        {
            auto type_id = typeIDOf< ComponentType >();
            return componentMap[type_id];
        }

        template < typename ComponentType >
        ComponentStorageHandler< ComponentType > *getComponentStorage()
        {
            auto type_id = typeIDOf< ComponentType >();
            auto component_interface = componentMap[type_id];

            return componentStorageCast< ComponentType >( component_interface->componentStorage );
        }

        template < typename ComponentType >
        const ComponentStorageHandler< ComponentType > *getComponentStorage() const
        {
            auto type_id = typeIDOf< ComponentType >();
            auto component_interface = componentMap[type_id];

            return componentStorageCast< ComponentType >( component_interface->componentStorage );
        }

        void callDestructors( ComponentInterface *component_interface, void *data );

      private:
        EntityHandleManager handleManager{};
        std::vector< ComponentInterface * > components{};
        std::unordered_map< compiled::TypeID, ComponentInterface * > componentMap{};
    };
}
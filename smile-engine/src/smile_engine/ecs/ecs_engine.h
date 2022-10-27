#pragma once

#include "entity_handle_manager.h"
#include "component_interface.h"
#include "field_detection.h"
#include "view.h"
#include "group.h"

#include "smile_engine/core/compiled/type_id.h"
#include "smile_engine/core/timestep.h"

namespace smile::ecs
{
    template < typename... Components >
    constexpr ComponentList< Components... > get{};

    class ECSEngine final
    {
      public:
        template < typename... Components >
        struct GatherComponents final
        {
          public:
            GatherComponents( ECSEngine &engine )
            {
                fillComponentInterfaces< 0, Components... >( engine );
            }

            bool run( EntityHandleType entity_handle )
            {
                for ( Uint32 i{}; i < size; ++i )
                {
                    components[i] = componentInterfaces[i]->getRaw( entity_handle );
                    if ( !components[i] )
                        return false;
                }

                return true;
            }

            template < typename Component >
            bool runComponent( Component &component )
            {
                return run( *reinterpret_cast< const EntityHandleType * >( &component ) );
            }

          private:
            template < Uint32 Index, typename Component, typename... Components >
            void fillComponentInterfaces( ECSEngine &engine )
            {
                componentInterfaces[Index] = engine.getComponentInterface< Component >();
                fillComponentInterfaces< Index + 1, Components... >( engine );
            }

            template < Uint32 Index >
            void fillComponentInterfaces( ECSEngine & )
            {
            }

          public:
            static constexpr Uint32 size = sizeof...( Components );
            ComponentInterface *componentInterfaces[size];
            void *components[size];
        };

      public:
        ECSEngine() = default;
        virtual ~ECSEngine();

        EntityHandleType createEntity()
        {
            return handleManager.createEntity();
        }

        void destroyEntity( EntityHandleType entity_handle )
        {
            handleManager.destroyEntity( entity_handle );

            for ( auto component_interface : components )
            {
                if ( hasComponent( component_interface, entity_handle ) )
                    removeComponent( component_interface, entity_handle );
            }
        }

        bool isEntityActive( EntityHandleType entity_handle ) const
        {
            return handleManager.isEntityActive( entity_handle );
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &addComponent( EntityHandleType entity_handle, ConstructorArgs &&...constructor_args )
        {
            registerComponentIfNeeded< ComponentType >();

            ComponentInterface *component_interface = getComponentInterface< ComponentType >();
            ComponentStorage *component_storage = getComponentStorage< ComponentType >();

            const IndexType index = component_interface->sparseSet.insert( entity_handle.index );

            SM_ASSERT( index == component_storage->getSize(), "ECSEngine::addComponent > Failed to add component" );

            ComponentType &component = component_storage->append< ComponentType >(
                entity_handle.index, std::forward< ConstructorArgs >( constructor_args )... );

            for ( auto &group : groups )
            {
                if ( group.hasComponent( component_interface ) )
                    group.addEntity( entity_handle.index );
            }

            return component;
        }

        template< typename ComponentType, typename... ConstructorArgs >
        ComponentType& addOrReplaceComponent(EntityHandleType entity_handle, ConstructorArgs&&... constructor_args)
        {
            ComponentInterface *component_interface = getComponentInterface< ComponentType >();
            if ( component_interface && component_interface->sparseSet.contains( entity_handle.index ) )
                removeComponent< ComponentType >( entity_handle );

            return addComponent< ComponentType >( entity_handle, std::forward< ConstructorArgs >( constructor_args )... );
        }

        template < typename ComponentType >
        void removeComponent( EntityHandleType entity_handle )
        {
            ComponentInterface *component_interface = getComponentInterface< ComponentType >();
            removeComponent( component_interface, entity_handle );
        }

        void removeComponent( ComponentInterface *component_interface, EntityHandleType entity_handle );

        template < typename ComponentType >
        ComponentType &getComponent( EntityHandleType entity_handle )
        {
            auto it = componentMap.find( typeIDOf< ComponentType >() );

            SM_ASSERT( it != componentMap.end(), "ECSEngine::getComponent > Component is missing" );

            return it->second->get< ComponentType >( entity_handle );
        }

        template < typename ComponentType >
        const ComponentType &getComponent( EntityHandleType entity_handle ) const
        {
            auto it = componentMap.find( typeIDOf< ComponentType >() );

            SM_ASSERT( it != componentMap.end(), "ECSEngine::getComponent > Component is missing" );

            return it->second->get< ComponentType >( entity_handle );
        }

        template < typename... Components >
        std::tuple< Components &... > getComponents( EntityHandleType entity_handle )
        {
            return std::tie( getComponent< Components >( entity_handle )... );
        }

        template < typename... Components >
        std::tuple< const Components &... > getComponents( EntityHandleType entity_handle ) const
        {
            return std::tie( getComponent< Components >( entity_handle )... );
        }

        template < typename Component >
        bool hasComponent( EntityHandleType entity_handle ) const
        {
            const ComponentInterface *component_interface = getComponentInterface< Component >();
            return component_interface ? component_interface->sparseSet.contains( entity_handle.index ) : false;
        }

        bool hasComponent( ComponentInterface *component_interface, EntityHandleType entity_handle ) const
        {
            return component_interface ? component_interface->sparseSet.contains( entity_handle.index ) : false;
        }

        template < typename... Components >
        View view()
        {
            const std::vector< ComponentInterface * > components{ getComponentInterface< Components >()... };

            for (auto component : components)
            {
                if ( !component )
                    return View{ handleManager, {} };
            }

            return View{ handleManager, components };
        }

        template < typename... Components, typename... Get >
        const Group &group( ComponentList< Get... > get = {} )
        {
            std::vector< ComponentInterface * > owned_components{ getComponentInterface< Components >()... };
            std::vector< ComponentInterface * > get_components{ getComponentInterface< Get >()... };
            
            auto it = std::find_if( groups.begin(),
                groups.end(),
                [owned_components, get_components]( const Group &group )
                {
                    const auto &group_owned = group.getOwnedComponents();
                    const auto &group_get = group.getGetComponents();

                    //for ( auto component : owned_components )
                    //{
                    //    auto it = std::find( group_owned.begin(), group_owned.end(), component );
                    //    SM_ASSERT( it == group_owned.end(), "ECSEngine::group > Component already owned by a group" );
                    //}

                    return owned_components.size() == group_owned.size() && get_components.size() == group_get.size() &&
                           std::equal( owned_components.begin(), owned_components.end(), group_owned.begin() ) &&
                           std::equal( get_components.begin(), get_components.end(), group_get.begin() );
                } );

            if ( it != groups.end() )
                return *it;

            Group group{ handleManager, owned_components, get_components };
            groups.push_back( group );
            return groups.back();
        }

        template < typename System, typename... Args >
        System *registerSystem( Args &&...args )
        {
            System *result = new System{ std::forward< Args >( args )... };

            if constexpr ( hasComponents< System > )
                registerComponentHelper< System >( result );

            if constexpr ( hasECSUpdate< ECSEngine, System, Timestep > )
                registerECSUpdate< System >( result );

            destructorHandlers.emplace_back( [=]() { delete result; } );

            return result;
        }

        template < typename FunctionType >
        void each( FunctionType function )
        {
            for ( const auto handle : handleManager )
                std::apply( function, std::make_tuple( handle ) );
        }

        void onUpdate( Timestep delta_time );
        void clear();

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

        template < typename System >
        void registerComponentHelper( System* system )
        {
            if constexpr ( hasECSComponentUpdate< ECSEngine, System, System::components, Timestep > )
                registerECSComponentUpdate< System >( system, System::components() );

            if constexpr ( hasComponentUpdate< System, System::components, Timestep > )
                registerComponentUpdate< System >( system, System::components() );

            if constexpr ( hasDestroy< System, list::Head< System::components > > )
                registerDestroy< System, list::Head< System::components > >( system );

            if constexpr ( hasCreate< System, list::Head< System::components >, EntityHandleType > )
                registerCreate< System, list::Head< System::components >, EntityHandleType >( system );
        }

        template < typename System, typename Component, typename... Components >
        void registerECSComponentUpdate( System *system, ComponentList< Component, Components... > && )
        {
            auto *component_storage = getComponentStorage< Component >();
            registerUpdate(
                [=]( Timestep delta_time )
                {
                    GatherComponents< 0, Components... > gather{ *this };
                    for ( auto &component : *component_storage )
                    {
                        if ( gather.runComponent( component ) )
                            system->onUpdate( *this, component, gather.template get< Components >()..., delta_time );
                    }
                } );
        }

        template < typename System, typename Component, typename... Components >
        void registerComponentUpdate( System *system, ComponentList< Component, Components... > && )
        {
            auto *component_storage = getComponentStorage< Component >();
            registerUpdate(
                [=]( Timestep delta_time )
                {
                    GatherComponents< 0, Components... > gather{ *this };
                    for ( auto &component : *component_storage )
                    {
                        if ( gather.runComponent( component ) )
                            system->onUpdate( component, gather.template get< Components >()..., delta_time );
                    }
                } );
        }

        template < typename System >
        void registerECSUpdate( System *system )
        {
            registerUpdate( [=]( Timestep delta_time ) { system->onUpdate( *this, delta_time ); } );
        }

        void registerUpdate( std::function< void( Timestep ) > update_handler )
        {
            updateHandlers.push_back( update_handler );
        }

        template < typename System, typename Component >
        void registerDestroy( System *system )
        {
            ComponentInterface *component_interface = getComponentInterface< Component >();

            SM_ASSERT( component_interface, "ECSEngine::registerDestroy > Unregistered component!" );

            component_interface->destroy.push_back(
                [=]( void *data ) { system->onDestroy( *reinterpret_cast< Component * >( data ) ); } );
        }

        template < typename System, typename Component, typename Handle >
        void registerCreate( System *system )
        {
            ComponentInterface *component_interface = getComponentInterface< Component >();

            SM_ASSERT( component_interface, "ECSEngine::registerDestroy > Unregistered component!" );

            component_interface->create.push_back( [=]( Handle entity_handle, void *data )
                { system->onCreate( entity_handle, *reinterpret_cast< Component * >( data ) ); } );
        }

        template < typename ComponentType >
        ComponentInterface *getComponentInterface()
        {
            auto type_id = typeIDOf< ComponentType >();
            return componentMap.find( type_id ) != componentMap.end() ? componentMap[type_id] : nullptr;
        }

        template < typename ComponentType >
        const ComponentInterface *getComponentInterface() const
        {
            auto type_id = typeIDOf< ComponentType >();
            return componentMap.find( type_id ) != componentMap.end() ? componentMap.at( type_id ) : nullptr;
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
        std::vector< Group > groups{};

        std::vector< std::function< void( Timestep ) > > updateHandlers{};
        std::vector< std::function< void() > > destructorHandlers{};
    };
}
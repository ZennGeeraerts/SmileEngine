#pragma once

#include "entity_handle_manager.h"
#include "component_interface.h"
#include "field_detection.h"
#include "view.h"
#include "group.h"

#include "smile_engine/core/compiled/type_id.h"
#include "smile_engine/core/timestep.h"

namespace Smile::ECS
{
    template < typename... Components >
    constexpr ComponentList< Components... > g_Get{};

    class ECSEngine final
    {
      public:
        template < typename... Components >
        struct GatherComponents final
        {
          public:
            GatherComponents( ECSEngine &engine )
            {
                FillComponentInterfaces< 0, Components... >( engine );
            }

            bool Run( EntityHandleType entityHandle )
            {
                for ( Uint32 i{}; i < s_Size; ++i )
                {
                    m_pComponents[i] = m_pComponentInterfaces[i]->GetRaw( entityHandle );
                    if ( !m_pComponents[i] )
                        return false;
                }

                return true;
            }

            template < typename Component >
            bool RunComponent( Component &component )
            {
                return Run( *reinterpret_cast< const EntityHandleType * >( &component ) );
            }

          private:
            template < Uint32 index, typename Component, typename... Components >
            void FillComponentInterfaces( ECSEngine &engine )
            {
                m_pComponentInterfaces[index] = engine.GetComponentInterface< Component >();
                FillComponentInterfaces< index + 1, Components... >( engine );
            }

            template < Uint32 index >
            void FillComponentInterfaces( ECSEngine & )
            {
            }

          public:
            static constexpr Uint32 s_Size = sizeof...( Components );
            ComponentInterface *m_pComponentInterfaces[s_Size];
            void *m_pComponents[s_Size];
        };

      public:
        ECSEngine() = default;
        virtual ~ECSEngine();

        EntityHandleType CreateEntity()
        {
            return m_HandleManager.CreateEntity();
        }

        void DestroyEntity( EntityHandleType entityHandle )
        {
            m_HandleManager.DestroyEntity( entityHandle );

            for ( auto pComponentInterface : m_pComponents )
            {
                if ( HasComponent( pComponentInterface, entityHandle ) )
                    RemoveComponent( pComponentInterface, entityHandle );
            }
        }

        bool IsEntityActive( EntityHandleType entityHandle ) const
        {
            return m_HandleManager.IsEntityActive( entityHandle );
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddComponent( EntityHandleType entityHandle, ConstructorArgs &&...constructorArgs )
        {
            RegisterComponentIfNeeded< ComponentType >();

            ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();
            ComponentStorage *pComponentStorage = GetComponentStorage< ComponentType >();

            const IndexType index = pComponentInterface->m_Pool.Insert( entityHandle.Index );

            SM_ASSERT( index == pComponentStorage->GetSize(), "ECSEngine::AddComponent > Failed to add component" );

            ComponentType &component = pComponentStorage->Append< ComponentType >(
                entityHandle.Index, std::forward< ConstructorArgs >( constructorArgs )... );

            for ( auto &group : m_Groups )
            {
                if ( group.HasComponent( pComponentInterface ) )
                    group.AddEntity( entityHandle.Index );
            }

            return component;
        }

        template< typename ComponentType, typename... ConstructorArgs >
        ComponentType& AddOrReplaceComponent(EntityHandleType entityHandle, ConstructorArgs&&... constructorArgs)
        {
            ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();
            if ( pComponentInterface && pComponentInterface->m_Pool.Contains( entityHandle.Index ) )
                RemoveComponent< ComponentType >( entityHandle );

            return AddComponent< ComponentType >( entityHandle, std::forward< ConstructorArgs >( constructorArgs )... );
        }

        template < typename ComponentType >
        void RemoveComponent( EntityHandleType entityHandle )
        {
            ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();
            RemoveComponent( pComponentInterface, entityHandle );
        }

        void RemoveComponent( ComponentInterface *pComponentInterface, EntityHandleType entityHandle );

        template < typename ComponentType >
        ComponentType &GetComponent( EntityHandleType entityHandle )
        {
            auto it = m_ComponentMap.find( TypeIDOf< ComponentType >() );

            SM_ASSERT( it != m_ComponentMap.end(), "ECSEngine::GetComponent > Component is missing" );

            return it->second->Get< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        const ComponentType &GetComponent( EntityHandleType entityHandle ) const
        {
            auto it = m_ComponentMap.find( TypeIDOf< ComponentType >() );

            SM_ASSERT( it != m_ComponentMap.end(), "ECSEngine::GetComponent > Component is missing" );

            return it->second->Get< ComponentType >( entityHandle );
        }

        template < typename... Components >
        std::tuple< Components &... > GetComponents( EntityHandleType entityHandle )
        {
            return std::tie( GetComponent< Components >( entityHandle )... );
        }

        template < typename... Components >
        std::tuple< const Components &... > GetComponents( EntityHandleType entityHandle ) const
        {
            return std::tie( GetComponent< Components >( entityHandle )... );
        }

        template < typename Component >
        bool HasComponent( EntityHandleType entityHandle ) const
        {
            const ComponentInterface *pComponentInterface = GetComponentInterface< Component >();
            return pComponentInterface ? pComponentInterface->m_Pool.Contains( entityHandle.Index ) : false;
        }

        bool HasComponent( ComponentInterface *pComponentInterface, EntityHandleType entityHandle ) const
        {
            return pComponentInterface ? pComponentInterface->m_Pool.Contains( entityHandle.Index ) : false;
        }

        template < typename... Components >
        View GetView()
        {
            const std::vector< ComponentInterface * > pComponents{ GetComponentInterface< Components >()... };

            for (auto pComponent : pComponents)
            {
                if ( !pComponent )
                    return View{ m_HandleManager, {} };
            }

            return View{ m_HandleManager, pComponents };
        }

        template < typename... Components, typename... Get >
        const Group &GetGroup( ComponentList< Get... > get = {} )
        {
            std::vector< ComponentInterface * > pOwnedComponents{ GetComponentInterface< Components >()... };
            std::vector< ComponentInterface * > pGetComponents{ GetComponentInterface< Get >()... };
            
            auto it = std::find_if( m_Groups.begin(),
                m_Groups.end(),
                [pOwnedComponents, pGetComponents]( const Group &group )
                {
                    const auto &groupOwned = group.GetOwnedComponents();
                    const auto &groupGet = group.GetGetComponents();

                    //for ( auto component : owned_components )
                    //{
                    //    auto it = std::find( group_owned.begin(), group_owned.end(), component );
                    //    SM_ASSERT( it == group_owned.end(), "ECSEngine::group > Component already owned by a group" );
                    //}

                    return pOwnedComponents.size() == groupOwned.size() && pGetComponents.size() == groupGet.size() &&
                           std::equal( pOwnedComponents.begin(), pOwnedComponents.end(), groupOwned.begin() ) &&
                           std::equal( pGetComponents.begin(), pGetComponents.end(), groupGet.begin() );
                } );

            if ( it != m_Groups.end() )
                return *it;

            Group group{ m_HandleManager, pOwnedComponents, pGetComponents };
            m_Groups.push_back( group );
            return m_Groups.back();
        }

        //template < typename System, typename... Args >
        //System *RegisterSystem( Args &&...args )
        //{
        //    System *pResult = new System{ std::forward< Args >( args )... };

        //    if constexpr ( hasComponents< System > )
        //        registerComponentHelper< System >( pResult );

        //    if constexpr ( hasECSUpdate< ECSEngine, System, Timestep > )
        //        registerECSUpdate< System >( pResult );

        //    destructorHandlers.emplace_back( [=]() { delete pResult; } );

        //    return pResult;
        //}

        template < typename FunctionType >
        void Each( FunctionType function )
        {
            for ( const auto handle : m_HandleManager )
                std::apply( function, std::make_tuple( handle ) );
        }

        //void onUpdate( Timestep delta_time );
        void Clear();

      private:
        template < typename ComponentType >
        void RegisterComponent( bool isRelational = false )
        {
            ComponentInterface *pComponentInterface = new ComponentInterface{};
            pComponentInterface->m_IsRelational = isRelational;
            pComponentInterface->m_pComponentStorage = new ComponentStorageHandler< ComponentType >( isRelational );

            m_pComponents.push_back( pComponentInterface );

            auto typeID = TypeIDOf< ComponentType >();
            m_ComponentMap[typeID] = pComponentInterface;
        }

        template < typename ComponentType >
        void RegisterComponentIfNeeded( bool isRelational = false )
        {
            auto typeID = TypeIDOf< ComponentType >();
            if ( m_ComponentMap.find( typeID ) == m_ComponentMap.end() )
                RegisterComponent< ComponentType >( isRelational );
        }

        /*template < typename System >
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
                        if ( gather.RunComponent( component ) )
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
                        if ( gather.RunComponent( component ) )
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

            component_interface->m_Destroy.push_back(
                [=]( void *data ) { system->onDestroy( *reinterpret_cast< Component * >( data ) ); } );
        }

        template < typename System, typename Component, typename Handle >
        void registerCreate( System *system )
        {
            ComponentInterface *component_interface = getComponentInterface< Component >();

            SM_ASSERT( component_interface, "ECSEngine::registerDestroy > Unregistered component!" );

            component_interface->m_Create.push_back( [=]( Handle entity_handle, void *data )
                { system->onCreate( entity_handle, *reinterpret_cast< Component * >( data ) ); } );
        }*/

        template < typename ComponentType >
        ComponentInterface *GetComponentInterface()
        {
            auto typeID = TypeIDOf< ComponentType >();
            return m_ComponentMap.find( typeID ) != m_ComponentMap.end() ? m_ComponentMap[typeID] : nullptr;
        }

        template < typename ComponentType >
        const ComponentInterface *GetComponentInterface() const
        {
            auto typeID = TypeIDOf< ComponentType >();
            return m_ComponentMap.find( typeID ) != m_ComponentMap.end() ? m_ComponentMap.at( typeID ) : nullptr;
        }

        template < typename ComponentType >
        ComponentStorageHandler< ComponentType > *GetComponentStorage()
        {
            auto typeID = TypeIDOf< ComponentType >();
            auto pComponentInterface = m_ComponentMap[typeID];

            return ComponentStorageCast< ComponentType >( pComponentInterface->m_pComponentStorage );
        }

        template < typename ComponentType >
        const ComponentStorageHandler< ComponentType > *GetComponentStorage() const
        {
            auto typeID = TypeIDOf< ComponentType >();
            auto pComponentInterface = m_ComponentMap[typeID];

            return ComponentStorageCast< ComponentType >( pComponentInterface->m_pComponentStorage );
        }

        void CallDestructors( ComponentInterface *pComponentInterface, void *pData );

      private:
        EntityHandleManager m_HandleManager{};
        std::vector< ComponentInterface * > m_pComponents{};
        std::unordered_map< Compiled::TypeID, ComponentInterface * > m_ComponentMap{};
        std::vector< Group > m_Groups{};

        /*std::vector< std::function< void( Timestep ) > > updateHandlers{};
        std::vector< std::function< void() > > destructorHandlers{};*/
    };
}
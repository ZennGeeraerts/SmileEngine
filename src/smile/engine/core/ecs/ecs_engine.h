/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "entity_handle_manager.h"
#include "component_interface.h"
#include "component_storage_handler.h"
#include "component_list.h"
#include "base_system.h"
#include "group_base.h"

#include "engine/common/compiled/type_id.h"

#include <algorithm>

namespace smile::ecs
{
    template < typename... Components >
    constexpr ComponentList< Components... > g_Get{};

    class ECSEngine final
    {
      public:
        template < typename... Components >
        class GatherComponents final
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
                    if ( !m_pComponentInterfaces[i] ||
                         !m_pComponentInterfaces[i]->m_Pool.Contains( entityHandle.GetIndex() ) )
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
            template < Uint32 Index, typename Component, typename... Components >
            void FillComponentInterfaces( ECSEngine &engine )
            {
                m_pComponentInterfaces[Index] = engine.GetComponentInterface< Component >();
                FillComponentInterfaces< Index + 1, Components... >( engine );
            }

            template < Uint32 Index >
            void FillComponentInterfaces( ECSEngine & )
            {
            }

          private:
            static constexpr Uint32 s_Size = sizeof...( Components );
            ComponentInterface *m_pComponentInterfaces[s_Size];
        };

        template < typename... Components >
        class ViewIterator final
        {
          public:
            ViewIterator( ECSEngine &engine, SparseSetType::ConstIterator it, SparseSetType::ConstIterator endIt )
                : m_Engine{ engine }, m_Iterator{ it }, m_EndIterator{ endIt }
            {
            }

            EntityHandleType operator*() const
            {
                return m_Engine.GetEntityHandleManager().GetEntityHandle( *m_Iterator );
            }

            bool operator==( const ViewIterator &other ) const
            {
                return m_Iterator == other.m_Iterator ||
                       ( *m_Iterator ) == m_Engine.GetEntityHandleManager().GetEntityCount();
            }

            bool operator!=( const ViewIterator &other ) const
            {
                return m_Iterator != other.m_Iterator &&
                       ( *m_Iterator ) != m_Engine.GetEntityHandleManager().GetEntityCount();
            }

            ViewIterator &operator++()
            {
                const EntityHandleManager &handleManager = m_Engine.GetEntityHandleManager();
                SparseSetType::ConstIterator oldIt;
                do
                {
                    oldIt = m_Iterator;

                    GatherComponents< Components... > gatherComponents{ m_Engine };
                    if ( gatherComponents.Run( handleManager.GetEntityHandle( *m_Iterator ) ) )
                    {
                        ++m_Iterator;
                        break;
                    }
                } while ( ( m_Iterator != m_EndIterator ) && ( m_Iterator != oldIt ) &&
                          ( handleManager.GetEntityHandle( *m_Iterator ).IsValid() ) );

                return *this;
            }

          private:
            ECSEngine &m_Engine;
            SparseSetType::ConstIterator m_Iterator;
            SparseSetType::ConstIterator m_EndIterator;
        };

        template < typename... Components >
        class View final
        {
          public:
            using ViewIteratorType = ViewIterator< Components... >;

          public:
            View( ECSEngine &engine ) : m_Engine{ engine }
            {
                const std::vector< ComponentInterface * > pComponents{
                    engine.GetComponentInterface< Components >()... };

                std::vector< SparseSetType * > pPools{};
                for ( auto pComponent : pComponents )
                {
                    if ( pComponent )
                        pPools.push_back( &pComponent->m_Pool );
                }

                auto it = std::min_element( std::begin( pPools ),
                    std::end( pPools ),
                    []( SparseSetType *pLhs, SparseSetType *pRhs )
                    { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

                m_pMinPool = ( it != pPools.end() ) ? *it : nullptr;
            }

            const ViewIteratorType begin() const
            {
                if ( m_pMinPool )
                {
                    return ViewIteratorType{ m_Engine, m_pMinPool->begin(), m_pMinPool->end() };
                }
                else
                {
                    return ViewIteratorType{ m_Engine, SparseSetType::ConstIterator{}, SparseSetType::ConstIterator{} };
                }
            }

            const ViewIteratorType end() const
            {
                if ( m_pMinPool )
                {
                    return ViewIteratorType{ m_Engine, m_pMinPool->end(), m_pMinPool->end() };
                }
                else
                {
                    return ViewIteratorType{ m_Engine, SparseSetType::ConstIterator{}, SparseSetType::ConstIterator{} };
                }
            }

          private:
            static constexpr Uint32 s_Size = sizeof...( Components );
            ECSEngine &m_Engine;
            SparseSetType *m_pMinPool;
        };

        template < typename... Components >
        class Group final : public GroupBase
        {
          public:
            Group( ECSEngine &engine,
                const std::vector< ComponentInterface * > &pOwned,
                const std::vector< ComponentInterface * > &pGet )
                : GroupBase{ engine }
            {
                std::vector< SparseSetType * > pPools{};

                for ( auto pComponent : pOwned )
                {
                    if ( pComponent )
                    {
                        pPools.push_back( &pComponent->m_Pool );
                        m_pOwnedPools.push_back( pComponent );
                    }
                }

                for ( auto pComponent : pGet )
                {
                    if ( pComponent )
                    {
                        pPools.push_back( &pComponent->m_Pool );
                        m_pGetPools.push_back( pComponent );
                    }
                }

                if ( pPools.empty() )
                    return;

                SparseSetType *pMinPool = *std::min_element( std::begin( pPools ),
                    std::end( pPools ),
                    []( SparseSetType *pLhs, SparseSetType *pRhs )
                    { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

                GatherComponents< Components... > gatherComponents{ m_Engine };
                for ( Uint32 i{}; i < pMinPool->GetItemCount(); ++i )
                {
                    auto entityIndex = pMinPool->GetElement( i );
                    auto entityHandle = m_Engine.GetEntityHandleManager().GetEntityHandle( entityIndex );

                    if ( gatherComponents.Run( entityHandle ) )
                    {
                        for ( auto pPool : m_pOwnedPools )
                        {
                            IndexType index = pPool->m_Pool.GetIndex( entityIndex );
                            pPool->m_Pool.Swap( pPool->m_Pool.GetElement( i ), entityIndex );
                            pPool->m_pComponentStorage->Swap( i, index );
                        }

                        ++m_EndIndex;
                    }
                }
            }

            bool HasEntity( IndexType entityIndex ) const override
            {
                GatherComponents< Components... > gatherComponents{ m_Engine };
                auto entityHandle = m_Engine.GetEntityHandleManager().GetEntityHandle( entityIndex );
                return gatherComponents.Run( entityHandle );
            }
        };

      public:
        ECSEngine() = default;
        ~ECSEngine();

        void OnUpdate();

        EntityHandleType CreateEntity()
        {
            return m_HandleManager.CreateEntity();
        }

        void DestroyEntity( EntityHandleType entityHandle );

        bool IsEntityActive( EntityHandleType entityHandle ) const
        {
            return m_HandleManager.IsEntityActive( entityHandle );
        }

        void MarkEntityForDelete( EntityHandleType entityHandle )
        {
            m_DeadHandles.push_back( entityHandle );
        }

        template < typename ComponentType >
        void RegisterComponent()
        {
            ComponentInterface *pComponentInterface = new ComponentInterface{};
            pComponentInterface->m_pComponentStorage = new ComponentStorageHandler< ComponentType >( *this );

            m_pComponents.push_back( pComponentInterface );

            auto typeID = compiled::TypeIDOf< ComponentType >();
            m_ComponentMap[typeID] = pComponentInterface;
        }

        template < typename ComponentType >
        void RegisterComponentIfNeeded()
        {
            auto typeID = compiled::TypeIDOf< ComponentType >();
            if ( m_ComponentMap.find( typeID ) == m_ComponentMap.end() )
                RegisterComponent< ComponentType >();
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddComponent( EntityHandleType entityHandle, ConstructorArgs &&...constructorArgs )
        {
            RegisterComponentIfNeeded< ComponentType >();

            ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();
            auto *pComponentStorage = GetComponentStorage< ComponentType >();

            const IndexType index = pComponentInterface->m_Pool.Insert( entityHandle.GetIndex() );

            SM_ASSERT( index == pComponentStorage->GetSize(), "ECSEngine::AddComponent > Failed to add component" );

            ComponentType &component = pComponentStorage->Append< ComponentType >(
                entityHandle.GetIndex(), std::forward< ConstructorArgs >( constructorArgs )... );

            for ( auto &pGroup : m_pGroups )
            {
                if ( pGroup->HasComponent( pComponentInterface ) )
                    pGroup->AddEntity( entityHandle );
            }

            return component;
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddOrReplaceComponent( EntityHandleType entityHandle, ConstructorArgs &&...constructorArgs )
        {
            ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();
            if ( pComponentInterface && pComponentInterface->m_Pool.Contains( entityHandle.GetIndex() ) )
                RemoveComponent< ComponentType >( entityHandle );

            return AddComponent< ComponentType >( entityHandle, std::forward< ConstructorArgs >( constructorArgs )... );
        }

        template < typename ComponentType >
        void RemoveComponent( EntityHandleType entityHandle )
        {
            ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();
            RemoveComponent( pComponentInterface, entityHandle );
        }

        template < typename ComponentType >
        ComponentType &GetComponent( EntityHandleType entityHandle )
        {
            auto it = m_ComponentMap.find( compiled::TypeIDOf< ComponentType >() );

            SM_ASSERT( it != m_ComponentMap.end(), "ECSEngine::GetComponent > Component is missing" );

            return it->second->Get< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        const ComponentType &GetComponent( EntityHandleType entityHandle ) const
        {
            auto it = m_ComponentMap.find( compiled::TypeIDOf< ComponentType >() );

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

        template < typename ComponentType >
        ComponentType *TryGetComponent( EntityHandleType entityHandle )
        {
            auto it = m_ComponentMap.find( compiled::TypeIDOf< ComponentType >() );

            if ( it == m_ComponentMap.end() )
                return nullptr;

            return it->second->TryGet< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        const ComponentType *TryGetComponent( EntityHandleType entityHandle ) const
        {
            auto it = m_ComponentMap.find( compiled::TypeIDOf< ComponentType >() );

            if ( it == m_ComponentMap.end() )
                return nullptr;

            return it->second->TryGet< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        bool HasComponent( EntityHandleType entityHandle ) const
        {
            const ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();
            return pComponentInterface ? pComponentInterface->m_Pool.Contains( entityHandle.GetIndex() ) : false;
        }

        template < typename ComponentType >
        void Reset()
        {
            ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();

            auto view = GetView< ComponentType >();
            for ( auto entity : view )
            {
                RemoveComponent( pComponentInterface, entity );
            }

            m_pComponents.erase( std::remove( m_pComponents.begin(), m_pComponents.end(), pComponentInterface ) );
            m_ComponentMap.erase( compiled::TypeIDOf< ComponentType >() );
        }

        template < typename ComponentType >
        bool IsComponentOwned() const
        {
            const ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();
            return IsComponentOwned( pComponentInterface );
        }

        template < typename... C >
        View< C... > GetView()
        {
            return View< C... >{ *this };
        }

        template < typename... Owned, typename... Get >
        Group< Owned..., Get... > GetGroup( ComponentList< Get... > get = {} )
        {
            ( RegisterComponentIfNeeded< Owned >(), ... );
            ( RegisterComponentIfNeeded< Get >(), ... );

            std::vector< ComponentInterface * > pOwnedComponents{ GetComponentInterface< Owned >()... };
            std::vector< ComponentInterface * > pGetComponents{ GetComponentInterface< Get >()... };

            auto it = std::find_if( m_pGroups.begin(),
                m_pGroups.end(),
                [pOwnedComponents, pGetComponents]( GroupBase *pGroup )
                {
                    const auto &pGroupOwned = pGroup->GetOwnedComponents();
                    const auto &pGroupGet = pGroup->GetGetComponents();

                    return pOwnedComponents.size() == pGroupOwned.size() && pGetComponents.size() == pGroupGet.size() &&
                           std::equal( pOwnedComponents.begin(), pOwnedComponents.end(), pGroupOwned.begin() ) &&
                           std::equal( pGetComponents.begin(), pGetComponents.end(), pGroupGet.begin() );
                } );

            if ( it != m_pGroups.end() )
                return *( static_cast< Group< Owned..., Get... > * >( *it ) );

            SM_ASSERT( std::none_of( pOwnedComponents.cbegin(),
                           pOwnedComponents.cend(),
                           [&]( ComponentInterface *pComponentInterface )
                           { return pComponentInterface && IsComponentOwned( pComponentInterface ); } ),
                "ECSEngine::GetGroup > Component(s) are already owned by a group" );

            GroupBase *pNewGroup = new Group< Owned..., Get... >{ *this, pOwnedComponents, pGetComponents };
            m_pGroups.push_back( pNewGroup );

            return *( static_cast< Group< Owned..., Get... > * >( pNewGroup ) );
        }

        void AddSystem( Ref< BaseSystem > pSystem );
        void RemoveSystem( Ref< BaseSystem > pSystem );

        template < typename FunctionType >
        void Each( FunctionType function )
        {
            for ( const auto handle : m_HandleManager )
                std::apply( function, std::make_tuple( handle ) );
        }

        void Clear();

        const EntityHandleManager &GetEntityHandleManager() const
        {
            return m_HandleManager;
        }

        template < typename ComponentType, typename Compare >
        void SortComponent( Compare compare )
        {
            ComponentInterface *pComponentInterface = GetComponentInterface< ComponentType >();

            SM_ASSERT(
                !IsComponentOwned( pComponentInterface ), "ECSEngine::SortComponent > Cannot sort owned component" );

            auto comp = [this, compare = std::move( compare )]( const IndexType lhs, const IndexType rhs )
            {
                const EntityHandleType lhsEntity = m_HandleManager.GetEntityHandle( lhs );
                const EntityHandleType rhsEntity = m_HandleManager.GetEntityHandle( rhs );
                return compare( lhsEntity, rhsEntity );
            };

            auto pool = pComponentInterface->m_Pool;
            auto &poolRef = pComponentInterface->m_Pool;

            std::sort( pool.m_Dense.begin(), pool.m_Dense.end(), std::move( comp ) );

            for ( std::size_t pos{}; pos < pool.GetItemCount(); ++pos )
            {
                auto curr = pos;
                auto next = poolRef.m_Sparse[pool.m_Dense[curr]];

                while ( curr != next )
                {
                    std::swap( poolRef.m_Dense[poolRef.m_Sparse[pool.m_Dense[curr]]],
                        poolRef.m_Dense[poolRef.m_Sparse[pool.m_Dense[next]]] );

                    pComponentInterface->m_pComponentStorage->Swap(
                        poolRef.m_Sparse[pool.m_Dense[curr]], poolRef.m_Sparse[pool.m_Dense[next]] );

                    poolRef.m_Sparse[pool.m_Dense[curr]] = curr;
                    curr = next;
                    next = poolRef.m_Sparse[pool.m_Dense[curr]];
                }
            }
        }

        template < typename ComponentType >
        auto &OnConstruction()
        {
            auto *pComponentStorage = GetComponentStorage< ComponentType >();
            return pComponentStorage->OnConstruction();
        }

        template < typename ComponentType >
        auto &OnDestruction()
        {
            auto *pComponentStorage = GetComponentStorage< ComponentType >();
            return pComponentStorage->OnDestruction();
        }

      private:
        template < typename ComponentType >
        ComponentInterface *GetComponentInterface()
        {
            auto typeID = compiled::TypeIDOf< ComponentType >();
            return m_ComponentMap.find( typeID ) != m_ComponentMap.end() ? m_ComponentMap[typeID] : nullptr;
        }

        template < typename ComponentType >
        const ComponentInterface *GetComponentInterface() const
        {
            auto typeID = compiled::TypeIDOf< ComponentType >();
            return m_ComponentMap.find( typeID ) != m_ComponentMap.end() ? m_ComponentMap.at( typeID ) : nullptr;
        }

        template < typename ComponentType >
        ComponentStorageHandler< ComponentType > *GetComponentStorage()
        {
            auto typeID = compiled::TypeIDOf< ComponentType >();
            auto pComponentInterface = m_ComponentMap[typeID];

            return ComponentStorageCast< ComponentType >( pComponentInterface->m_pComponentStorage );
        }

        template < typename ComponentType >
        const ComponentStorageHandler< ComponentType > *GetComponentStorage() const
        {
            auto typeID = compiled::TypeIDOf< ComponentType >();
            auto pComponentInterface = m_ComponentMap[typeID];

            return ComponentStorageCast< ComponentType >( pComponentInterface->m_pComponentStorage );
        }

        void RemoveComponent( ComponentInterface *pComponentInterface, EntityHandleType entityHandle );
        bool HasComponent( ComponentInterface *pComponentInterface, EntityHandleType entityHandle ) const;
        bool IsComponentOwned( const ComponentInterface *pComponentInterface ) const;

        void CallDestructors( ComponentInterface *pComponentInterface, void *pData );

      private:
        EntityHandleManager m_HandleManager{};
        std::vector< ComponentInterface * > m_pComponents{};
        std::unordered_map< compiled::TypeID, ComponentInterface * > m_ComponentMap{};
        std::vector< GroupBase * > m_pGroups{};
        std::vector< Ref< BaseSystem > > m_pSystems{};
        std::vector< EntityHandleType > m_DeadHandles{};
    };
}
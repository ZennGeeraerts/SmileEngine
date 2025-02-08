/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "entity_handle_manager.h"
#include "component_pool.h"
#include "component_storage_handler.h"
#include "component_list.h"
#include "base_system.h"
#include "group_base.h"

#include "engine/common/foundation/type_id.h"

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
                FillComponentPools< 0, Components... >( engine );
            }

            bool Run( EntityHandleType entityHandle )
            {
                for ( Uint32 i{}; i < s_Size; ++i )
                {
                    if ( !m_pComponentPools[i] || !m_pComponentPools[i]->Contains( entityHandle ) )
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
            void FillComponentPools( ECSEngine &engine )
            {
                m_pComponentPools[Index] = engine.GetComponentPool< Component >();
                FillComponentPools< Index + 1, Components... >( engine );
            }

            template < Uint32 Index >
            void FillComponentPools( ECSEngine & )
            {
            }

          private:
            static constexpr Uint32 s_Size = sizeof...( Components );
            ComponentPool *m_pComponentPools[s_Size];
        };

        template < typename... Components >
        class ViewIterator final
        {
          public:
            ViewIterator( ECSEngine &engine, ComponentPool::ConstIterator it, ComponentPool::ConstIterator endIt )
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
                ComponentPool::ConstIterator oldIt;
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
            ComponentPool::ConstIterator m_Iterator;
            ComponentPool::ConstIterator m_EndIterator;
        };

        template < typename... Components >
        class View final
        {
          public:
            using ViewIteratorType = ViewIterator< Components... >;

          public:
            View( ECSEngine &engine ) : m_Engine{ engine }
            {
                std::vector< ComponentPool * > pPools{ engine.GetComponentPool< Components >()... };

                // Remove non existent component interfaces
                pPools.erase(
                    std::remove_if(
                        std::begin( pPools ), std::end( pPools ), []( const ComponentPool *pPool ) { return !pPool; } ),
                    std::end( pPools ) );

                auto it = std::min_element( std::begin( pPools ),
                    std::end( pPools ),
                    []( const ComponentPool *pLhs, const ComponentPool *pRhs )
                    { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

                m_pSmallestPool = ( it != pPools.end() ) ? *it : nullptr;
            }

            const ViewIteratorType begin() const
            {
                if ( m_pSmallestPool )
                {
                    return ViewIteratorType{ m_Engine, m_pSmallestPool->begin(), m_pSmallestPool->end() };
                }
                else
                {
                    return ViewIteratorType{ m_Engine, SparseSetType::ConstIterator{}, SparseSetType::ConstIterator{} };
                }
            }

            const ViewIteratorType end() const
            {
                if ( m_pSmallestPool )
                {
                    return ViewIteratorType{ m_Engine, m_pSmallestPool->end(), m_pSmallestPool->end() };
                }
                else
                {
                    return ViewIteratorType{ m_Engine, SparseSetType::ConstIterator{}, SparseSetType::ConstIterator{} };
                }
            }

          private:
            static constexpr Uint32 s_Size = sizeof...( Components );
            ECSEngine &m_Engine;
            ComponentPool *m_pSmallestPool;
        };

        template < typename... Components >
        class Group final : public GroupBase
        {
          public:
            Group( ECSEngine &engine,
                const std::vector< ComponentPool * > &pOwned,
                const std::vector< ComponentPool * > &pGet )
                : GroupBase{ engine, pOwned, pGet }
            {
                std::vector< ComponentPool * > pPools{};
                pPools.reserve( pOwned.size() + pGet.size() );
                pPools.insert( pPools.end(), pOwned.begin(), pOwned.end() );
                pPools.insert( pPools.end(), pGet.begin(), pGet.end() );

                if ( pPools.empty() )
                    return;

                const ComponentPool *pSmallestPool = *std::min_element( std::begin( pPools ),
                    std::end( pPools ),
                    []( const ComponentPool *pLhs, const ComponentPool *pRhs )
                    { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

                GatherComponents< Components... > gatherComponents{ m_Engine };
                for ( IndexType i{}; i < pSmallestPool->GetItemCount(); ++i )
                {
                    auto entityHandle = pSmallestPool->GetEntityHandle( i );

                    if ( gatherComponents.Run( entityHandle ) )
                    {
                        for ( auto pPool : m_pOwnedPools )
                        {
                            pPool->Swap( i, pPool->GetIndex( entityHandle ) );
                        }

                        ++m_EndIndex;
                    }
                }
            }

            bool ContainsEntity( EntityHandleType entityHandle ) const override
            {
                GatherComponents< Components... > gatherComponents{ m_Engine };
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
            ComponentPool *pPool = new ComponentPool{ *this };
            pPool->m_pComponentStorage = new ComponentStorageHandler< ComponentType >( *this );

            m_pComponentPools.push_back( pPool );

            auto typeID = foundation::TypeIDOf< ComponentType >();
            m_ComponentPoolMap[typeID] = pPool;
        }

        template < typename ComponentType >
        void RegisterComponentIfNeeded()
        {
            auto typeID = foundation::TypeIDOf< ComponentType >();
            if ( m_ComponentPoolMap.find( typeID ) == m_ComponentPoolMap.end() )
                RegisterComponent< ComponentType >();
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddComponent( EntityHandleType entityHandle, ConstructorArgs &&...constructorArgs )
        {
            RegisterComponentIfNeeded< ComponentType >();

            ComponentPool *pPool = GetComponentPool< ComponentType >();
            auto *pComponentStorage = GetComponentStorage< ComponentType >();

            const IndexType index = pPool->m_SparseSet.Insert( entityHandle.GetIndex() );

            SM_ASSERT( index == pComponentStorage->GetSize(), "ECSEngine::AddComponent > Failed to add component" );

            ComponentType &component = pComponentStorage->Append< ComponentType >(
                entityHandle.GetIndex(), std::forward< ConstructorArgs >( constructorArgs )... );

            for ( auto &pGroup : m_pGroups )
            {
                if ( pGroup->ContainsComponentPool( pPool ) )
                    pGroup->AddEntity( entityHandle );
            }

            return component;
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddOrReplaceComponent( EntityHandleType entityHandle, ConstructorArgs &&...constructorArgs )
        {
            ComponentPool *pPool = GetComponentPool< ComponentType >();
            if ( pPool && pPool->Contains( entityHandle ) )
                RemoveComponent< ComponentType >( entityHandle );

            return AddComponent< ComponentType >( entityHandle, std::forward< ConstructorArgs >( constructorArgs )... );
        }

        template < typename ComponentType >
        void RemoveComponent( EntityHandleType entityHandle )
        {
            ComponentPool *pPool = GetComponentPool< ComponentType >();
            RemoveComponent( pPool, entityHandle );
        }

        template < typename ComponentType >
        ComponentType &GetComponent( EntityHandleType entityHandle )
        {
            auto it = m_ComponentPoolMap.find( foundation::TypeIDOf< ComponentType >() );

            SM_ASSERT( it != m_ComponentPoolMap.end(), "ECSEngine::GetComponent > Component is missing" );

            return it->second->Get< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        const ComponentType &GetComponent( EntityHandleType entityHandle ) const
        {
            auto it = m_ComponentPoolMap.find( foundation::TypeIDOf< ComponentType >() );

            SM_ASSERT( it != m_ComponentPoolMap.end(), "ECSEngine::GetComponent > Component is missing" );

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
            auto it = m_ComponentPoolMap.find( foundation::TypeIDOf< ComponentType >() );

            if ( it == m_ComponentPoolMap.end() )
                return nullptr;

            return it->second->TryGet< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        const ComponentType *TryGetComponent( EntityHandleType entityHandle ) const
        {
            auto it = m_ComponentPoolMap.find( foundation::TypeIDOf< ComponentType >() );

            if ( it == m_ComponentPoolMap.end() )
                return nullptr;

            return it->second->TryGet< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        bool HasComponent( EntityHandleType entityHandle ) const
        {
            const ComponentPool *pPool = GetComponentPool< ComponentType >();
            return pPool ? pPool->Contains( entityHandle ) : false;
        }

        template < typename ComponentType >
        void Reset()
        {
            ComponentPool *pPool = GetComponentPool< ComponentType >();

            auto view = GetView< ComponentType >();
            for ( auto entity : view )
            {
                RemoveComponent( pPool, entity );
            }

            m_pComponentPools.erase( std::remove( m_pComponentPools.begin(), m_pComponentPools.end(), pPool ) );
            m_ComponentPoolMap.erase( foundation::TypeIDOf< ComponentType >() );
        }

        template < typename ComponentType >
        bool IsComponentOwned() const
        {
            const ComponentPool *pPool = GetComponentPool< ComponentType >();
            return IsComponentOwned( pPool );
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

            std::vector< ComponentPool * > pOwnedComponents{ GetComponentPool< Owned >()... };
            std::vector< ComponentPool * > pGetComponents{ GetComponentPool< Get >()... };

            auto it = std::find_if( m_pGroups.begin(),
                m_pGroups.end(),
                [pOwnedComponents, pGetComponents]( GroupBase *pGroup )
                {
                    const auto &pGroupOwned = pGroup->GetOwnedPools();
                    const auto &pGroupGet = pGroup->GetGetPools();

                    return pOwnedComponents.size() == pGroupOwned.size() && pGetComponents.size() == pGroupGet.size() &&
                           std::equal( pOwnedComponents.begin(), pOwnedComponents.end(), pGroupOwned.begin() ) &&
                           std::equal( pGetComponents.begin(), pGetComponents.end(), pGroupGet.begin() );
                } );

            if ( it != m_pGroups.end() )
                return *( static_cast< Group< Owned..., Get... > * >( *it ) );

            SM_ASSERT( std::none_of( pOwnedComponents.cbegin(),
                           pOwnedComponents.cend(),
                           [&]( const ComponentPool *pPool ) { return pPool && IsComponentOwned( pPool ); } ),
                "ECSEngine::GetGroup > Component pool(s) are already owned by a group" );

            GroupBase *pNewGroup = new Group< Owned..., Get... >{ *this, pOwnedComponents, pGetComponents };
            m_pGroups.push_back( pNewGroup );

            return *( static_cast< Group< Owned..., Get... > * >( pNewGroup ) );
        }

        void AddSystem( Ref< BaseSystem > pSystem );
        void RemoveSystem( Ref< BaseSystem > pSystem );

        const std::vector< Ref< BaseSystem > > &GetSystems() const
        {
            return m_pSystems;
        }

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
            ComponentPool *pPool = GetComponentPool< ComponentType >();

            SM_ASSERT( !IsComponentOwned( pPool ), "ECSEngine::SortComponent > Cannot sort owned component" );

            auto comp = [this, compare = std::move( compare )]( const IndexType lhs, const IndexType rhs )
            {
                const EntityHandleType lhsEntity = m_HandleManager.GetEntityHandle( lhs );
                const EntityHandleType rhsEntity = m_HandleManager.GetEntityHandle( rhs );
                return compare( lhsEntity, rhsEntity );
            };

            auto pool = pPool->m_SparseSet;
            auto &poolRef = pPool->m_SparseSet;

            std::sort( pool.m_Dense.begin(), pool.m_Dense.end(), std::move( comp ) );

            for ( std::size_t pos{}; pos < pool.GetItemCount(); ++pos )
            {
                auto curr = pos;
                auto next = poolRef.m_Sparse[pool.m_Dense[curr]];

                while ( curr != next )
                {
                    std::swap( poolRef.m_Dense[poolRef.m_Sparse[pool.m_Dense[curr]]],
                        poolRef.m_Dense[poolRef.m_Sparse[pool.m_Dense[next]]] );

                    pPool->m_pComponentStorage->Swap(
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
        ComponentPool *GetComponentPool()
        {
            auto typeID = foundation::TypeIDOf< ComponentType >();
            return m_ComponentPoolMap.find( typeID ) != m_ComponentPoolMap.end() ? m_ComponentPoolMap[typeID] : nullptr;
        }

        template < typename ComponentType >
        const ComponentPool *GetComponentPool() const
        {
            auto typeID = foundation::TypeIDOf< ComponentType >();
            return m_ComponentPoolMap.find( typeID ) != m_ComponentPoolMap.end() ? m_ComponentPoolMap.at( typeID )
                                                                                 : nullptr;
        }

        template < typename ComponentType >
        ComponentStorageHandler< ComponentType > *GetComponentStorage()
        {
            auto typeID = foundation::TypeIDOf< ComponentType >();
            auto pPool = m_ComponentPoolMap[typeID];

            return ComponentStorageCast< ComponentType >( pPool->m_pComponentStorage );
        }

        template < typename ComponentType >
        const ComponentStorageHandler< ComponentType > *GetComponentStorage() const
        {
            auto typeID = foundation::TypeIDOf< ComponentType >();
            auto pPool = m_ComponentPoolMap[typeID];

            return ComponentStorageCast< ComponentType >( pPool->m_pComponentStorage );
        }

        void RemoveComponent( ComponentPool *pPool, EntityHandleType entityHandle );
        bool HasComponent( const ComponentPool *pPool, EntityHandleType entityHandle ) const;
        bool IsComponentOwned( const ComponentPool *pPool ) const;

        void CallDestructors( ComponentPool *pPool, void *pData );

      private:
        EntityHandleManager m_HandleManager{};
        std::vector< ComponentPool * > m_pComponentPools{};
        std::unordered_map< foundation::TypeID, ComponentPool * > m_ComponentPoolMap{};
        std::vector< GroupBase * > m_pGroups{};
        std::vector< Ref< BaseSystem > > m_pSystems{};
        std::vector< EntityHandleType > m_DeadHandles{};
    };
}
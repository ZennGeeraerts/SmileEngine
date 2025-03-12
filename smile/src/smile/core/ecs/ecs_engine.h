/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "entity_handle.h"
#include "component_pool.h"
#include "component_list.h"
#include "base_system.h"
#include "group_base.h"

#include "smile/common/foundation/type_id.h"

#include "smile/common/memory/ref.h"

#include <algorithm>
#include <array>

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

            bool Run( EntityHandle entityHandle )
            {
                return std::all_of( std::begin( m_pComponentPools ),
                    std::end( m_pComponentPools ),
                    [entityHandle]( const ComponentPool *pCPool )
                    { return pCPool && pCPool->Contains( entityHandle ); } );
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
            std::array< ComponentPool *, s_Size > m_pComponentPools;
        };

        template < typename... Components >
        class ViewIterator final
        {
          public:
            ViewIterator( ECSEngine &engine, ComponentPool::ConstIterator it, ComponentPool::ConstIterator endIt )
                : m_Engine{ engine }, m_Iterator{ it }, m_EndIterator{ endIt }
            {
            }

            EntityHandle operator*() const
            {
                return m_Engine.GetEntityHandleManager().GetHandle( *m_Iterator );
            }

            bool operator==( const ViewIterator &other ) const
            {
                return m_Iterator == other.m_Iterator ||
                       ( *m_Iterator ) == m_Engine.GetEntityHandleManager().GetHandleCount();
            }

            bool operator!=( const ViewIterator &other ) const
            {
                return m_Iterator != other.m_Iterator &&
                       ( *m_Iterator ) != m_Engine.GetEntityHandleManager().GetHandleCount();
            }

            ViewIterator &operator++()
            {
                const EntityHandleManager &handleManager = m_Engine.GetEntityHandleManager();
                ComponentPool::ConstIterator oldIt;
                do
                {
                    oldIt = m_Iterator;

                    GatherComponents< Components... > gatherComponents{ m_Engine };
                    if ( gatherComponents.Run( handleManager.GetHandle( *m_Iterator ) ) )
                    {
                        ++m_Iterator;
                        break;
                    }
                } while ( ( m_Iterator != m_EndIterator ) && ( m_Iterator != oldIt ) &&
                          ( handleManager.GetHandle( *m_Iterator ).IsValid() ) );

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
                std::vector< ComponentPool * > pCPools{ engine.GetComponentPool< Components >()... };

                // Remove non existent component interfaces
                pCPools.erase( std::remove_if( std::begin( pCPools ),
                                   std::end( pCPools ),
                                   []( const ComponentPool *pCPool ) { return !pCPool; } ),
                    std::end( pCPools ) );

                auto it = std::min_element( std::begin( pCPools ),
                    std::end( pCPools ),
                    []( const ComponentPool *pLhs, const ComponentPool *pRhs )
                    { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

                m_pSmallestPool = ( it != pCPools.end() ) ? *it : nullptr;
            }

            const ViewIteratorType begin() const
            {
                if ( m_pSmallestPool )
                {
                    return ViewIteratorType{ m_Engine, m_pSmallestPool->begin(), m_pSmallestPool->end() };
                }
                else
                {
                    return ViewIteratorType{ m_Engine, ComponentPool::ConstIterator{}, ComponentPool::ConstIterator{} };
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
                    return ViewIteratorType{ m_Engine, ComponentPool::ConstIterator{}, ComponentPool::ConstIterator{} };
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
                std::vector< ComponentPool * > pCPools{};
                pCPools.reserve( pOwned.size() + pGet.size() );
                pCPools.insert( pCPools.end(), pOwned.begin(), pOwned.end() );
                pCPools.insert( pCPools.end(), pGet.begin(), pGet.end() );

                if ( pCPools.empty() )
                    return;

                const ComponentPool *pSmallestCPool = *std::min_element( std::begin( pCPools ),
                    std::end( pCPools ),
                    []( const ComponentPool *pLhs, const ComponentPool *pRhs )
                    { return pLhs->GetItemCount() < pRhs->GetItemCount(); } );

                GatherComponents< Components... > gatherComponents{ m_Engine };
                for ( IndexType i{}; i < pSmallestCPool->GetItemCount(); ++i )
                {
                    auto entityHandle = pSmallestCPool->GetEntityHandle( i );

                    if ( gatherComponents.Run( entityHandle ) )
                    {
                        for ( auto pCPool : m_pOwnedPools )
                        {
                            pCPool->Swap( i, pCPool->GetIndex( entityHandle ) );
                        }

                        ++m_EndIndex;
                    }
                }
            }

            bool ContainsEntity( EntityHandle entityHandle ) const override
            {
                GatherComponents< Components... > gatherComponents{ m_Engine };
                return gatherComponents.Run( entityHandle );
            }
        };

      public:
        ECSEngine() = default;
        ~ECSEngine();

        void OnUpdate();

        EntityHandle CreateEntity()
        {
            return m_HandleManager.CreateHandle();
        }

        void DestroyEntity( EntityHandle entityHandle );

        bool IsEntityActive( EntityHandle entityHandle ) const
        {
            return m_HandleManager.IsHandleActive( entityHandle );
        }

        void MarkEntityForDelete( EntityHandle entityHandle )
        {
            m_DeadHandles.push_back( entityHandle );
        }

        template < typename ComponentType >
        void RegisterComponent()
        {
            ComponentPool *pCPool = new ComponentPool{ *this };
            pCPool->Initialize< ComponentType >();

            m_pComponentPools.push_back( pCPool );

            auto typeID = foundation::TypeIDOf< ComponentType >();
            m_ComponentPoolMap[typeID] = pCPool;
        }

        template < typename ComponentType >
        void RegisterComponentIfNeeded()
        {
            auto typeID = foundation::TypeIDOf< ComponentType >();
            if ( m_ComponentPoolMap.find( typeID ) == m_ComponentPoolMap.end() )
                RegisterComponent< ComponentType >();
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddComponent( EntityHandle entityHandle, ConstructorArgs &&...constructorArgs )
        {
            RegisterComponentIfNeeded< ComponentType >();

            ComponentPool *pCPool = GetComponentPool< ComponentType >();
            ComponentType &component =
                pCPool->Add< ComponentType >( entityHandle, std::forward< ConstructorArgs >( constructorArgs )... );

            for ( auto &pGroup : m_pGroups )
            {
                if ( pGroup->ContainsComponentPool( pCPool ) )
                    pGroup->AddEntity( entityHandle );
            }

            return component;
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &AddOrReplaceComponent( EntityHandle entityHandle, ConstructorArgs &&...constructorArgs )
        {
            ComponentPool *pCPool = GetComponentPool< ComponentType >();
            if ( pCPool && pCPool->Contains( entityHandle ) )
                RemoveComponent< ComponentType >( entityHandle );

            return AddComponent< ComponentType >( entityHandle, std::forward< ConstructorArgs >( constructorArgs )... );
        }

        template < typename ComponentType >
        void RemoveComponent( EntityHandle entityHandle )
        {
            ComponentPool *pCPool = GetComponentPool< ComponentType >();
            RemoveComponent( pCPool, entityHandle );
        }

        template < typename ComponentType >
        ComponentType &GetComponent( EntityHandle entityHandle )
        {
            auto it = m_ComponentPoolMap.find( foundation::TypeIDOf< ComponentType >() );

            SM_ASSERT( it != m_ComponentPoolMap.end(), "ECSEngine::GetComponent > Component is missing" );

            return it->second->Get< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        const ComponentType &GetComponent( EntityHandle entityHandle ) const
        {
            auto it = m_ComponentPoolMap.find( foundation::TypeIDOf< ComponentType >() );

            SM_ASSERT( it != m_ComponentPoolMap.end(), "ECSEngine::GetComponent > Component is missing" );

            return it->second->Get< ComponentType >( entityHandle );
        }

        template < typename... Components >
        std::tuple< Components &... > GetComponents( EntityHandle entityHandle )
        {
            return std::tie( GetComponent< Components >( entityHandle )... );
        }

        template < typename... Components >
        std::tuple< const Components &... > GetComponents( EntityHandle entityHandle ) const
        {
            return std::tie( GetComponent< Components >( entityHandle )... );
        }

        template < typename ComponentType >
        ComponentType *TryGetComponent( EntityHandle entityHandle )
        {
            auto it = m_ComponentPoolMap.find( foundation::TypeIDOf< ComponentType >() );

            if ( it == m_ComponentPoolMap.end() )
                return nullptr;

            return it->second->TryGet< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        const ComponentType *TryGetComponent( EntityHandle entityHandle ) const
        {
            auto it = m_ComponentPoolMap.find( foundation::TypeIDOf< ComponentType >() );

            if ( it == m_ComponentPoolMap.end() )
                return nullptr;

            return it->second->TryGet< ComponentType >( entityHandle );
        }

        template < typename ComponentType >
        bool HasComponent( EntityHandle entityHandle ) const
        {
            const ComponentPool *pCPool = GetComponentPool< ComponentType >();
            return pCPool ? pCPool->Contains( entityHandle ) : false;
        }

        template < typename ComponentType >
        void Reset()
        {
            ComponentPool *pCPool = GetComponentPool< ComponentType >();

            auto view = GetView< ComponentType >();
            for ( auto entity : view )
            {
                RemoveComponent( pCPool, entity );
            }

            m_pComponentPools.erase( std::remove( m_pComponentPools.begin(), m_pComponentPools.end(), pCPool ) );
            m_ComponentPoolMap.erase( foundation::TypeIDOf< ComponentType >() );
        }

        template < typename ComponentType >
        bool IsComponentOwned() const
        {
            const ComponentPool *pCPool = GetComponentPool< ComponentType >();
            return IsComponentOwned( pCPool );
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
                           [&]( const ComponentPool *pCPool ) { return pCPool && IsComponentOwned( pCPool ); } ),
                "ECSEngine::GetGroup > Component pool(s) are already owned by a group" );

            GroupBase *pNewGroup = new Group< Owned..., Get... >{ *this, pOwnedComponents, pGetComponents };
            m_pGroups.push_back( pNewGroup );

            return *( static_cast< Group< Owned..., Get... > * >( pNewGroup ) );
        }

        void AddSystem( memory::Ref< BaseSystem > pSystem );
        void RemoveSystem( memory::Ref< BaseSystem > pSystem );

        const std::vector< memory::Ref< BaseSystem > > &GetSystems() const
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
            ComponentPool *pCPool = GetComponentPool< ComponentType >();

            SM_ASSERT( !IsComponentOwned( pCPool ), "ECSEngine::SortComponent > Cannot sort owned component" );

            auto comp = [this, compare = std::move( compare )]( const IndexType lhs, const IndexType rhs )
            {
                const EntityHandle lhsEntity = m_HandleManager.GetHandle( lhs );
                const EntityHandle rhsEntity = m_HandleManager.GetHandle( rhs );
                return compare( lhsEntity, rhsEntity );
            };

            pCPool->Sort( comp );
        }

        template < typename ComponentType >
        auto &OnConstruction()
        {
            auto *pCPool = GetComponentPool< ComponentType >();
            return pCPool->OnConstruction();
        }

        template < typename ComponentType >
        auto &OnDestruction()
        {
            auto *pCPool = GetComponentPool< ComponentType >();
            return pCPool->OnDestruction();
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

        void RemoveComponent( ComponentPool *pCPool, EntityHandle entityHandle );
        bool HasComponent( const ComponentPool *pCPool, EntityHandle entityHandle ) const;
        bool IsComponentOwned( const ComponentPool *pCPool ) const;

      private:
        EntityHandleManager m_HandleManager{};
        std::vector< ComponentPool * > m_pComponentPools{};
        std::unordered_map< foundation::TypeID, ComponentPool * > m_ComponentPoolMap{};
        std::vector< GroupBase * > m_pGroups{};
        std::vector< memory::Ref< BaseSystem > > m_pSystems{};
        std::vector< EntityHandle > m_DeadHandles{};
    };
}
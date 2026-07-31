/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/sparse_set.h"
#include "smile/common/memory/scope.h"
#include "smile/common/primitive/collection/array_utils.h"
#include "smile/core/application/timer.h"
#include "component_storage_handler.h"

#include <functional>

namespace smile::ecs
{
    class ECSEngine;

    class ComponentPool final
    {
      public:
        using SparseSetType = typename primitive::SparseSet< IndexType >;
        using Iterator = SparseSetType::Iterator;
        using ConstIterator = SparseSetType::ConstIterator;

      private:
        using ListenerType = std::function< void( ECSEngine &, EntityHandle ) >;
        using ListenerContainer = std::vector< ListenerType >;

        struct Metadata final
        {
            Uint64 Created;
            Uint64 LastModified;
        };

      public:
        explicit ComponentPool( ECSEngine &ecsEngine ) noexcept;
        ~ComponentPool() noexcept = default;

        template < typename ComponentType >
        void Initialize()
        {
            SM_ASSERT_MSG( !m_ComponentStorage, "ComponentPool::Initialize > Storage already created" );

            m_ComponentStorage = memory::CreateScope< ComponentStorageHandler< ComponentType > >();
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &Add( EntityHandle entityHandle, ConstructorArgs &&...constructorArgs )
        {
            [[maybe_unused]] const IndexType index = m_SparseSet.Insert( entityHandle.GetIndex() );

            SM_ASSERT_MSG( index == m_ComponentStorage->GetSize(), "ComponentPool::Add > Failed to add component" );

            auto &component = m_ComponentStorage->Append< ComponentType >(
                entityHandle.GetIndex(), std::forward< ConstructorArgs >( constructorArgs )... );

            const Uint64 currentTick = application::Timer::GetInstance().GetTicks();
            m_ComponentMetadata.PushBack( { currentTick, currentTick } );

            PublishOnConstruction( entityHandle );

            return component;
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &Replace( EntityHandle entityHandle, ConstructorArgs &&...constructorArgs )
        {
            SM_ASSERT( Contains( entityHandle ) );

            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );

            auto &component = m_ComponentStorage->Replace< ComponentType >(
                index, std::forward< ConstructorArgs >( constructorArgs )... );

            Patch< ComponentType >( entityHandle );

            return component;
        }

        void Remove( EntityHandle entityHandle );

        template < typename ComponentType, typename... Func >
        void Patch( EntityHandle entityHandle, Func &&...func )
        {
            SM_ASSERT( Contains( entityHandle ) );

            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );
            m_ComponentMetadata[index].LastModified = application::Timer::GetInstance().GetTicks();

            auto &component = m_ComponentStorage->Get< ComponentType >( index );
            ( std::invoke( std::forward< Func >( func ), component ), ... );
            PublishOnPatch( entityHandle );
        }

        template < typename ComponentType >
        ComponentType &Get( EntityHandle entityHandle )
        {
            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );

            SM_ASSERT_MSG( index != EntityHandle::NullHandle().GetIndex(), "ComponentPool::Get > Invalid index" );

            return m_ComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        const ComponentType &Get( EntityHandle entityHandle ) const
        {
            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );
            return m_ComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        ComponentType *TryGet( EntityHandle entityHandle )
        {
            if ( !m_SparseSet.Contains( entityHandle.GetIndex() ) )
                return nullptr;

            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );

            if ( index == EntityHandle::NullHandle().GetIndex() )
                return nullptr;

            return &m_ComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        const ComponentType *TryGet( EntityHandle entityHandle ) const
        {
            if ( !m_SparseSet.Contains( entityHandle.GetIndex() ) )
                return nullptr;

            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );

            if ( index == EntityHandle::NullHandle().GetIndex() )
                return nullptr;

            return &m_ComponentStorage->Get< ComponentType >( index );
        }

        void *GetRaw( EntityHandle entityHandle )
        {
            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );
            return m_ComponentStorage->GetRaw( index );
        }

        const void *GetRaw( EntityHandle entityHandle ) const
        {
            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );
            return m_ComponentStorage->GetRaw( index );
        }

        void Clear() noexcept
        {
            m_ComponentStorage->Clear();
            m_SparseSet.Clear();
            m_ComponentMetadata.Clear();
        }

        bool Contains( EntityHandle entityHandle ) const
        {
            return m_SparseSet.Contains( entityHandle.GetIndex() );
        }

        void Swap( IndexType lhs, IndexType rhs )
        {
            m_SparseSet.Swap( m_SparseSet.GetElement( lhs ), m_SparseSet.GetElement( rhs ) );
            m_ComponentStorage->Swap( lhs, rhs );
            primitive::array::Swap( m_ComponentMetadata, lhs, rhs );
        }

        Count GetItemCount() const noexcept
        {
            return m_SparseSet.GetItemCount();
        }

        IndexType GetIndex( EntityHandle entityHandle ) const
        {
            return m_SparseSet.GetIndex( entityHandle.GetIndex() );
        }

        EntityHandle GetEntityHandle( IndexType index ) const;
        void Sort( std::function< bool( const IndexType, const IndexType ) > compare );

        ListenerContainer &OnConstruction()
        {
            return m_ConstructionListeners;
        }

        ListenerContainer &OnDestruction()
        {
            return m_DestructionListeners;
        }

        ListenerContainer &OnPatch()
        {
            return m_PatchListeners;
        }

        ConstIterator begin() const
        {
            return m_SparseSet.begin();
        }

        ConstIterator end() const
        {
            return m_SparseSet.end();
        }

      private:
        void PublishOnConstruction( const EntityHandle entityHandle );
        void PublishOnDestruction( const EntityHandle entityHandle );
        void PublishOnPatch( const EntityHandle entityHandle );

      private:
        ECSEngine &m_ECSEngine;
        SparseSetType m_SparseSet{};
        memory::Scope< ComponentStorage > m_ComponentStorage;
        primitive::Vector< Metadata > m_ComponentMetadata;

        ListenerContainer m_ConstructionListeners;
        ListenerContainer m_DestructionListeners;
        ListenerContainer m_PatchListeners;
    };
}
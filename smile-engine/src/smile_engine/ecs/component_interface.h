#pragma once

#include "smile_engine/stl/sparse_set.h"
#include "component_storage.h"

#include <functional>

namespace Smile::ECS
{
    using createHandler = std::function< void( EntityHandleType, void * ) >;
    using destroyHandler = std::function< void( void * ) >;

    class ComponentInterface final
    {
      public:
        virtual ~ComponentInterface()
        {
            SAFE_DELETE( m_pComponentStorage );
        }

        template < typename ComponentType >
        ComponentType &Get( EntityHandleType entity_handle )
        {
            const IndexType index = m_Pool.GetIndex( entity_handle.Index );

            SM_ASSERT( index != g_NullHandle< IndexType >.Index, "ComponentInterface::Get > Invalid index" );

            return m_pComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        const ComponentType &Get( EntityHandleType entity_handle ) const
        {
            const IndexType index = m_Pool.GetIndex( entity_handle.Index );
            return m_pComponentStorage->Get< ComponentType >( index );
        }

        void *GetRaw( EntityHandleType entity_handle )
        {
            const IndexType index = m_Pool.GetIndex( entity_handle.Index );
            return m_pComponentStorage->GetRaw( index );
        }

        const void *GetRaw( EntityHandleType entity_handle ) const
        {
            const IndexType index = m_Pool.GetIndex( entity_handle.Index );
            return m_pComponentStorage->GetRaw( index );
        }

        void Clear()
        {
            m_pComponentStorage->Clear();
            m_Pool.Clear();
        }

      public:
        bool m_IsRelational;
        SparseSetType m_Pool{};
        ComponentStorage *m_pComponentStorage;

        std::vector< createHandler > m_Create;
        std::vector< destroyHandler > m_Destroy;
    };
}
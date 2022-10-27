#pragma once

#include "smile_engine/stl/sparse_set.h"
#include "component_storage.h"

#include <functional>

namespace smile::ecs
{
    using createHandler = std::function< void( EntityHandleType, void * ) >;
    using destroyHandler = std::function< void( void * ) >;

    class ComponentInterface final
    {
      public:
        virtual ~ComponentInterface()
        {
            SAFE_DELETE( componentStorage );
        }

        template < typename ComponentType >
        ComponentType &get( EntityHandleType entity_handle )
        {
            const IndexType index = sparseSet.getIndex( entity_handle.index );

            SM_ASSERT( index != nullHandle< IndexType >.index, "ComponentInterface::get > Invalid index" );

            return componentStorage->get< ComponentType >( index );
        }

        template < typename ComponentType >
        const ComponentType &get( EntityHandleType entity_handle ) const
        {
            const IndexType index = sparseSet.getIndex( entity_handle.index );
            return componentStorage->get< ComponentType >( index );
        }

        void *getRaw( EntityHandleType entity_handle )
        {
            const IndexType index = sparseSet.getIndex( entity_handle.index );
            return componentStorage->getRaw( index );
        }

        const void *getRaw( EntityHandleType entity_handle ) const
        {
            const IndexType index = sparseSet.getIndex( entity_handle.index );
            return componentStorage->getRaw( index );
        }

        void clear()
        {
            componentStorage->clear();
            sparseSet.clear();
        }

      public:
        bool relational;
        SparseSetType sparseSet{};
        ComponentStorage *componentStorage;

        std::vector< createHandler > create;
        std::vector< destroyHandler > destroy;
    };
}
#pragma once

#include "entity_handle.h"

#include <vector>

namespace smile::ecs
{
    class EntityHandleManager final
    {
      public:
        EntityHandleManager() = default;

        EntityHandle createEntity();
        void destroyEntity( EntityHandle entity_handle );
        bool isEntityActive( EntityHandle entity_handle ) const;

        EntityHandle getEntityHandle( Uint32 index ) const
        {
            SM_ASSERT( index < entities.size(), "EntityHandleManager::getEntityHandle > Index out of range" );
            return entities[index];
        }

      private:
        std::vector< EntityHandle > entities{};
        Uint32 availableEntities{ 0 };
        Uint32 nextFreeEntityIndex{ std::numeric_limits< Uint32 >::max() }; // Used for implicit list
    };
}
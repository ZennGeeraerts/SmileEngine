#pragma once

#include "ecs_types.h"

#include <vector>

namespace smile::ecs
{
    class EntityHandleManager final
    {
      public:
        struct Iterator final
        {
            Iterator( const std::vector< EntityHandleType > &entities,
                std::vector< EntityHandleType >::const_iterator it )
                : entities{ entities }, it{ it }
            {
            }

            EntityHandleType operator*() const
            {
                return *it;
            }
            bool operator==( const Iterator &other ) const
            {
                return it == other.it;
            }
            bool operator!=( const Iterator &other ) const
            {
                return it != other.it;
            }

            Iterator &operator++()
            {
                do
                {
                    ++it;
                } while (
                    it != entities.end() && ( it->index > entities.size() || entities[it->index].index != it->index ) );

                return *this;
            }

            const std::vector< EntityHandleType > &entities;
            std::vector< EntityHandleType >::const_iterator it;
        };

      public:
        EntityHandleManager() = default;

        EntityHandleType createEntity();
        void destroyEntity( EntityHandleType entity_handle );
        bool isEntityActive( EntityHandleType entity_handle ) const;

        EntityHandleType getEntityHandle( IndexType index ) const
        {
            SM_ASSERT( index < entities.size(), "EntityHandleManager::getEntityHandle > Index out of range" );
            return entities[index];
        }

        Uint32 getEntityCount() const
        {
            return entities.size();
        }

        Iterator begin() const
        {
            auto it = entities.begin();
            while ( it != entities.end() && ( it->index > entities.size() || entities[it->index].index != it->index ) )
            {
                ++it;
            }

            return Iterator{ entities, it };
        }
        Iterator end() const
        {
            return Iterator{ entities, entities.end() };
        }

      private:
        std::vector< EntityHandleType > entities{};
        Uint32 availableEntities{ 0 };
        IndexType nextFreeEntityIndex{ std::numeric_limits< IndexType >::max() }; // Used for implicit list
    };
}
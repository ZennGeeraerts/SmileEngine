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
                : Entities{ entities }, It{ it }
            {
            }

            EntityHandleType operator*() const
            {
                return *It;
            }
            bool operator==( const Iterator &other ) const
            {
                return It == other.It;
            }
            bool operator!=( const Iterator &other ) const
            {
                return It != other.It;
            }

            Iterator &operator++()
            {
                do
                {
                    ++It;
                } while (
                    It != Entities.end() && ( It->GetIndex() > Entities.size() || Entities[It->GetIndex()].GetIndex() != It->GetIndex() ) );

                return *this;
            }

            const std::vector< EntityHandleType > &Entities;
            std::vector< EntityHandleType >::const_iterator It;
        };

      public:
        EntityHandleManager() = default;

        EntityHandleType CreateEntity();
        void DestroyEntity( EntityHandleType entityHandle );
        bool IsEntityActive( EntityHandleType entityHandle ) const;

        EntityHandleType GetEntityHandle( IndexType index ) const
        {
            SM_ASSERT( index < m_Entities.size(), "EntityHandleManager::getEntityHandle > Index out of range" );
            return m_Entities[index];
        }

        Uint32 GetEntityCount() const
        {
            return static_cast< Uint32 >( m_Entities.size() );
        }

        Iterator begin() const
        {
            auto it = m_Entities.begin();
            while ( it != m_Entities.end() &&
                    ( it->GetIndex() > m_Entities.size() || m_Entities[it->GetIndex()].GetIndex() != it->GetIndex() ) )
            {
                ++it;
            }

            return Iterator{ m_Entities, it };
        }
        Iterator end() const
        {
            return Iterator{ m_Entities, m_Entities.end() };
        }

      private:
        std::vector< EntityHandleType > m_Entities{};
        Uint32 m_AvailableEntities{ 0 };
        IndexType m_NextFreeEntityIndex{ std::numeric_limits< IndexType >::max() }; // Used for implicit list
    };
}
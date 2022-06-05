#pragma once

#include <stdint.h>
#include <numeric>

namespace smile::ecs
{
    struct EntityHandle final
    {
        static const int s_IndexBits = 16;
        static const int s_GenerationBits = 16;

        EntityHandle()
            : m_Index{ std::numeric_limits< uint32_t >::max() }, m_Generation{ std::numeric_limits< uint32_t >::max() }
        {
        }

        EntityHandle( uint32_t index, uint32_t counter ) : m_Index{ index }, m_Generation{ counter }
        {
        }

        bool isValid() const
        {
            return !( m_Index == std::numeric_limits< uint32_t >::max() &&
                      ( m_Generation == std::numeric_limits< uint32_t >::max() ) );
        }

        operator bool() const
        {
            return isValid();
        }
        bool operator==( const EntityHandle &rhs ) const
        {
            return m_Index == rhs.m_Index && m_Generation == rhs.m_Generation;
        }
        bool operator!=( const EntityHandle &rhs ) const
        {
            return m_Index != rhs.m_Index || m_Generation != rhs.m_Generation;
        }

        uint32_t m_Index : s_IndexBits;
        uint32_t m_Generation : s_GenerationBits;
    };
}

namespace std
{
    template <>
    struct hash< smile::ecs::EntityHandle >
    {
        size_t operator()( smile::ecs::EntityHandle entity_handle ) const
        {
            return entity_handle.m_Generation << entity_handle.s_IndexBits | entity_handle.m_Index;
        }
    };
}
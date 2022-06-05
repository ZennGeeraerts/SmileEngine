#pragma once

#include <vector>

namespace smile::ecs
{
	struct ComponentConfiguration final
    {
        bool operator==( const ComponentConfiguration &other ) const
        {
            return m_Types.size() == other.m_Types.size() &&
                   std::equal( std::begin( m_Types ), std::end( m_Types ), std::begin( other.m_Types ) );
        }

        void Sort();
        bool HasAnyTypes( const std::vector< size_t > &typeIDs );
        bool HasAllTypes( const std::vector< size_t > &typeIDs );

        std::vector< size_t > m_Types{};
    };
}
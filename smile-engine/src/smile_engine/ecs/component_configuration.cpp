#include "smpch.h"
#include "component_configuration.h"

namespace smile::ecs
{
    void ComponentConfiguration::Sort()
    {
        std::sort( m_Types.begin(), m_Types.end() );
    }

    bool ComponentConfiguration::HasAnyTypes(const std::vector< size_t >& typeIDs)
    {
        for ( auto id : typeIDs )
        {
            if ( std::find( m_Types.begin(), m_Types.end(), id ) != m_Types.end() )
            {
                return true;
            }
        }

        return false;
    }

    bool ComponentConfiguration::HasAllTypes( const std::vector< size_t > &typeIDs )
    {
        for ( auto id : typeIDs )
        {
            if ( std::find( m_Types.begin(), m_Types.end(), id ) == m_Types.end() )
            {
                return false;
            }
        }

        return true;
    }
}
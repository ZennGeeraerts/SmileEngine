#pragma once

#include "smile_engine/core/core.h"

#include <unordered_map>

namespace smile::ecs
{
    class ECSEngine;

    class ComponentDataGroup final
    {
        ComponentDataGroup( ECSEngine &engine );
        ~ComponentDataGroup();

        template < typename Type >
        Type &Get( const Int32 index )
        {
            constexpr auto tid = typeid_of_< Type >();
        }

     private:
        ECSEngine &m_Engine;
        std::unordered_map<>
    };
}
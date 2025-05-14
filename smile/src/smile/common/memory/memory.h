#pragma once

namespace smile::memory
{
    // Rounds up a given size to a multiple of the given alignment
    template < typename Type >
    Type Align( Type size, Type alignment )
    {
        return ( size + alignment - 1 ) & ~( alignment - 1 );
    }
}
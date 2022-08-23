#pragma once

#include "smile_engine/core/core.h"

namespace smile::ecs
{
    struct EntityHandle final
    {
        static const int indexBits = 16;
        static const int generationBits = 16;

        EntityHandle()
            : index{ std::numeric_limits< Uint32 >::max() }, generation{ std::numeric_limits< Uint32 >::max() }
        {
        }

        EntityHandle( Uint32 index, Uint32 generation ) : index{ index }, generation{ generation }
        {
        }

        bool isValid() const
        {
            return !( index == std::numeric_limits< Uint16 >::max() &&
                      ( generation == std::numeric_limits< Uint16 >::max() ) );
        }

        operator bool() const
        {
            return isValid();
        }
        bool operator==( const EntityHandle &rhs ) const
        {
            return index == rhs.index && generation == rhs.generation;
        }
        bool operator!=( const EntityHandle &rhs ) const
        {
            return index != rhs.index || generation != rhs.generation;
        }

        Uint32 index : indexBits;
        Uint32 generation : generationBits;
    };

    static const EntityHandle nullHandle = EntityHandle{};
}

namespace std
{
    template <>
    struct hash< smile::ecs::EntityHandle >
    {
        size_t operator()( smile::ecs::EntityHandle entity_handle ) const
        {
            return entity_handle.generation << entity_handle.indexBits | entity_handle.index;
        }
    };
}
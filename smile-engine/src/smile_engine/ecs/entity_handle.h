#pragma once

#include "smile_engine/core/core.h"

namespace smile::ecs
{
    template < typename IndexType = Uint32, Uint32 IndexBits = 16, Uint32 GenerationBits = 16 >
    struct EntityHandle final
    {
        static constexpr Uint32 indexBits = IndexBits;
        static constexpr Uint32 generationBits = GenerationBits;
        static constexpr IndexType maxIndex = ( 1 << IndexBits ) - 1;
        static constexpr IndexType maxGeneration = ( 1 << GenerationBits ) - 1;

        EntityHandle()
            : index{ std::numeric_limits< IndexType >::max() }, generation{ std::numeric_limits< IndexType >::max() }
        {
        }

        EntityHandle( IndexType index, IndexType generation ) : index{ index }, generation{ generation }
        {
        }

        size_t hash() const
        {
            return generation << indexBits | index;
        }
        bool isValid() const
        {
            return !( ( index == maxIndex ) || ( generation == maxGeneration ) );
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

        IndexType index : IndexBits;
        IndexType generation : GenerationBits;
    };

    template< typename IndexType >
    const EntityHandle< IndexType > nullHandle = EntityHandle< IndexType >{};
}

namespace std
{
    template < typename IndexType, smile::Uint32 IndexBits, smile::Uint32 GenerationBits >
    struct hash< smile::ecs::EntityHandle< IndexType, IndexBits, GenerationBits > >
    {
        size_t operator()( smile::ecs::EntityHandle< IndexType > handle ) const
        {
            return handle.hash();
        }
    };
}
#pragma once

#include "smile_engine/core/core.h"

namespace Smile::ECS
{
    template < typename IndexType = Uint32, Uint32 indexBits = 16, Uint32 generationBits = 16 >
    struct EntityHandle final
    {
        static constexpr Uint32 IndexBits = indexBits;
        static constexpr Uint32 GenerationBits = generationBits;
        static constexpr IndexType MaxIndex = ( 1 << IndexBits ) - 1;
        static constexpr IndexType MaxGeneration = ( 1 << GenerationBits ) - 1;

        EntityHandle()
            : Index{ std::numeric_limits< IndexType >::max() }, Generation{ std::numeric_limits< IndexType >::max() }
        {
        }

        EntityHandle( IndexType index, IndexType generation ) : Index{ index }, Generation{ generation }
        {
        }

        size_t Hash() const
        {
            return Generation << IndexBits | Index;
        }
        bool IsValid() const
        {
            return !( ( Index == MaxIndex ) || ( Generation == MaxGeneration ) );
        }

        operator bool() const
        {
            return IsValid();
        }
        bool operator==( const EntityHandle &rhs ) const
        {
            return Index == rhs.Index && Generation == rhs.Generation;
        }
        bool operator!=( const EntityHandle &rhs ) const
        {
            return Index != rhs.Index || Generation != rhs.Generation;
        }

        IndexType Index : IndexBits;
        IndexType Generation : GenerationBits;
    };

    template< typename IndexType >
    const EntityHandle< IndexType > g_NullHandle = EntityHandle< IndexType >{};
}

namespace std
{
    template < typename IndexType, Smile::Uint32 indexBits, Smile::Uint32 generationBits >
    struct hash< Smile::ECS::EntityHandle< IndexType, indexBits, generationBits > >
    {
        size_t operator()( Smile::ECS::EntityHandle< IndexType > entityHandle ) const
        {
            return entityHandle.Hash();
        }
    };
}
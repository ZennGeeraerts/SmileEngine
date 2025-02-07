/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/compiled.h"
#include "foundation/hash_code.h"

namespace smile::ecs
{
    template < typename IndexType = Uint32, Uint32 IndexBits = 16, Uint32 GenerationBits = 16 >
    class EntityHandle final
    {
      public:
        static constexpr Uint32 s_IndexBits = IndexBits;
        static constexpr Uint32 s_GenerationBits = GenerationBits;
        static constexpr IndexType s_MaxIndex = ( 1 << s_IndexBits ) - 1;
        static constexpr IndexType s_MaxGeneration = ( 1 << s_GenerationBits ) - 1;

      public:
        EntityHandle()
            : m_Index{ std::numeric_limits< IndexType >::max() },
              m_Generation{ std::numeric_limits< IndexType >::max() }
        {
        }

        EntityHandle( IndexType index, IndexType generation ) : m_Index{ index }, m_Generation{ generation }
        {
        }

        foundation::HashCode Hash() const
        {
            return m_Generation << s_IndexBits | m_Index;
        }
        bool IsValid() const
        {
            return !( ( m_Index == s_MaxIndex ) || ( m_Generation == s_MaxGeneration ) );
        }

        IndexType GetIndex() const
        {
            return m_Index;
        }

        operator bool() const
        {
            return IsValid();
        }
        bool operator==( const EntityHandle &rhs ) const
        {
            return m_Index == rhs.m_Index && m_Generation == rhs.m_Generation;
        }
        bool operator!=( const EntityHandle &rhs ) const
        {
            return m_Index != rhs.m_Index || m_Generation != rhs.m_Generation;
        }

        static constexpr EntityHandle< IndexType, IndexBits, GenerationBits > NullHandle();

      private:
        IndexType m_Index : s_IndexBits;
        IndexType m_Generation : s_GenerationBits;

        friend class EntityHandleManager;
    };

    template < typename IndexType, Uint32 IndexBits, Uint32 GenerationBits >
    constexpr EntityHandle< IndexType, IndexBits, GenerationBits > EntityHandle< IndexType, IndexBits, GenerationBits >::NullHandle()
    {
        return EntityHandle< IndexType, IndexBits, GenerationBits >{};
    }
}

namespace std
{
    template < typename IndexType, smile::Uint32 IndexBits, smile::Uint32 GenerationBits >
    struct hash< smile::ecs::EntityHandle< IndexType, IndexBits, GenerationBits > >
    {
        size_t operator()( smile::ecs::EntityHandle< IndexType, IndexBits, GenerationBits > entityHandle ) const
        {
            return entityHandle.Hash();
        }
    };
}
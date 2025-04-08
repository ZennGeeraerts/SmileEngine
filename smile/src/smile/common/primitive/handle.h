/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/hash_code.h"

namespace smile::primitive
{
    template < typename IndexType, Uint32 IndexBits, Uint32 GenerationBits >
    class HandleManager;

    template < typename IndexType = Uint32, Uint32 IndexBits = 16, Uint32 GenerationBits = 16 >
    class Handle final
    {
      public:
        static constexpr Uint32 s_IndexBits = IndexBits;
        static constexpr Uint32 s_GenerationBits = GenerationBits;
        static constexpr IndexType s_MaxIndex = ( 1 << s_IndexBits ) - 1;
        static constexpr IndexType s_MaxGeneration = ( 1 << s_GenerationBits ) - 1;

      public:
        Handle()
            : m_Index{ std::numeric_limits< IndexType >::max() },
              m_Generation{ std::numeric_limits< IndexType >::max() }
        {
        }

        Handle( IndexType index, IndexType generation ) : m_Index{ index }, m_Generation{ generation }
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
        bool operator==( const Handle &rhs ) const
        {
            return m_Index == rhs.m_Index && m_Generation == rhs.m_Generation;
        }
        bool operator!=( const Handle &rhs ) const
        {
            return m_Index != rhs.m_Index || m_Generation != rhs.m_Generation;
        }

        static constexpr Handle< IndexType, IndexBits, GenerationBits > NullHandle();

      private:
        IndexType m_Index : s_IndexBits;
        IndexType m_Generation : s_GenerationBits;

        friend class HandleManager< IndexType, IndexBits, GenerationBits >;
    };

    template < typename IndexType, Uint32 IndexBits, Uint32 GenerationBits >
    constexpr Handle< IndexType, IndexBits, GenerationBits >
    Handle< IndexType, IndexBits, GenerationBits >::NullHandle()
    {
        return Handle< IndexType, IndexBits, GenerationBits >{};
    }
}

namespace std
{
    template < typename IndexType, smile::Uint32 IndexBits, smile::Uint32 GenerationBits >
    struct hash< smile::primitive::Handle< IndexType, IndexBits, GenerationBits > >
    {
        smile::foundation::HashCode operator()(
            smile::primitive::Handle< IndexType, IndexBits, GenerationBits > handle ) const
        {
            return handle.Hash();
        }
    };
}
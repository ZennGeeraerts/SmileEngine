/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/hash_code.h"

namespace smile::primitive
{
    template < typename IndexType, Uint32 IndexBits, Uint32 GenerationBits, typename Tag >
    class HandleManager;

    template < typename IndexType = Uint32, Uint32 IndexBits = 16, Uint32 GenerationBits = 16, typename Tag = void >
    class Handle final
    {
      public:
        static constexpr Uint32 s_IndexBits = IndexBits;
        static constexpr Uint32 s_GenerationBits = GenerationBits;
        static constexpr IndexType s_MaxIndex = ( IndexType{ 1 } << s_IndexBits ) - 1;
        static constexpr IndexType s_MaxGeneration = ( IndexType{ 1 } << s_GenerationBits ) - 1;

      public:
        constexpr Handle() noexcept
            : m_Index{ std::numeric_limits< IndexType >::max() },
              m_Generation{ std::numeric_limits< IndexType >::max() }
        {
        }

        constexpr Handle( const IndexType index, const IndexType generation ) noexcept
            : m_Index{ index }, m_Generation{ generation }
        {
        }

        foundation::HashCode Hash() const noexcept
        {
            return m_Generation << s_IndexBits | m_Index;
        }

        constexpr bool IsValid() const noexcept
        {
            return !( ( m_Index == s_MaxIndex ) || ( m_Generation == s_MaxGeneration ) );
        }

        constexpr IndexType GetIndex() const noexcept
        {
            return m_Index;
        }

        operator bool() const noexcept
        {
            return IsValid();
        }

        constexpr bool operator==( const Handle &rhs ) const noexcept
        {
            return m_Index == rhs.m_Index && m_Generation == rhs.m_Generation;
        }

        constexpr bool operator!=( const Handle &rhs ) const noexcept
        {
            return m_Index != rhs.m_Index || m_Generation != rhs.m_Generation;
        }

        static constexpr Handle< IndexType, IndexBits, GenerationBits, Tag > NullHandle();

      private:
        IndexType m_Index : s_IndexBits;
        IndexType m_Generation : s_GenerationBits;

        friend class HandleManager< IndexType, IndexBits, GenerationBits, Tag >;
    };

    template < typename IndexType, Uint32 IndexBits, Uint32 GenerationBits, typename Tag >
    inline constexpr Handle< IndexType, IndexBits, GenerationBits, Tag >
    Handle< IndexType, IndexBits, GenerationBits, Tag >::NullHandle()
    {
        return Handle< IndexType, IndexBits, GenerationBits, Tag >{};
    }
}

namespace std
{
    template < typename IndexType, smile::Uint32 IndexBits, smile::Uint32 GenerationBits, typename Tag >
    struct hash< smile::primitive::Handle< IndexType, IndexBits, GenerationBits, Tag > >
    {
        smile::foundation::HashCode operator()(
            smile::primitive::Handle< IndexType, IndexBits, GenerationBits, Tag > handle ) const
        {
            return handle.Hash();
        }
    };
}
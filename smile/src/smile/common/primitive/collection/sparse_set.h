/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "vector.h"

#include <numeric>
#include <initializer_list>

namespace smile::primitive
{
    template < typename IndexType >
    class SparseSet final
    {
        static_assert( std::is_unsigned< IndexType >::value, "SparseSet can only contain unsigned integers" );

      public:
        using Iterator = typename ArrayIterator< Vector< IndexType >, false >;
        using ConstIterator = typename ArrayIterator< Vector< IndexType >, true >;

        SparseSet() = default;
        SparseSet( std::initializer_list< IndexType > elements )
        {
            for ( IndexType element : elements )
                Insert( element );
        }

        ~SparseSet()
        {
            Clear();
        }

        IndexType Insert( IndexType element )
        {
            SM_ASSERT_MSG( !Contains( element ), "SparseSet::Insert > Sparse set already contains this value" );

            const auto pos = static_cast< IndexType >( m_Dense.GetItemCount() );
            m_Dense.PushBack( element );

            if ( element >= m_Sparse.GetItemCount() )
                m_Sparse.SetItemCount( element + 1, std::numeric_limits< IndexType >::max() );

            m_Sparse[element] = pos;

            return m_Dense.GetItemCount() - 1;
        }

        IndexType Erase( IndexType element )
        {
            SM_ASSERT_MSG( Contains( element ), "SparseSet::Erase > Sparse set doesn't contain this value" );

            IndexType deadIndex = m_Sparse[element];

            const IndexType last = m_Dense.GetLastItem();
            std::swap( m_Dense.GetLastItem(), m_Dense[deadIndex] );
            std::swap( m_Sparse[last], m_Sparse[element] );
            m_Dense.PopBack();

            m_Sparse[element] = std::numeric_limits< IndexType >::max();

            return deadIndex;
        }

        bool Contains( IndexType element ) const
        {
            return ( element < m_Sparse.GetItemCount() ) && ( m_Sparse[element] < m_Dense.GetItemCount() ) &&
                   ( m_Sparse[element] != std::numeric_limits< IndexType >::max() );
        }

        void Clear()
        {
            m_Sparse.Clear();
            m_Dense.Clear();
        }

        void Swap( IndexType lhs, IndexType rhs )
        {
            SM_ASSERT_MSG(
                Contains( lhs ) && Contains( rhs ), "SparseSet::Swap > Sparse set doesn't contains this value" );

            if ( lhs == rhs )
                return;

            std::swap( m_Dense[m_Sparse[lhs]], m_Dense[m_Sparse[rhs]] );
            std::swap( m_Sparse[lhs], m_Sparse[rhs] );
        }

        template < typename Compare >
        void Sort( Compare compare )
        {
            Vector< IndexType > copy( m_Dense.GetItemCount() );
            std::iota( copy.begin(), copy.end(), IndexType{} );

            std::sort( copy.begin(),
                copy.end(),
                [this, compare = std::move( compare )]( const IndexType lhs, const IndexType rhs )
                { return compare( m_Dense[lhs], m_Dense[rhs] ); } );

            for ( IndexType pos{}; pos < copy.GetItemCount(); ++pos )
            {
                auto curr = pos;
                auto next = copy[curr];

                while ( curr != next )
                {
                    std::swap( m_Dense[copy[curr]], m_Dense[copy[next]] );
                    std::swap( m_Sparse[m_Dense[copy[curr]]], m_Sparse[m_Dense[copy[next]]] );

                    copy[curr] = curr;
                    curr = next;
                    next = copy[curr];
                }
            }
        }

        Iterator begin()
        {
            return m_Dense.begin();
        }

        Iterator end()
        {
            return m_Dense.end();
        }

        ConstIterator begin() const
        {
            return m_Dense.begin();
        }

        ConstIterator end() const
        {
            return m_Dense.end();
        }

        IndexType GetIndex( IndexType element ) const
        {
            SM_ASSERT_MSG( Contains( element ), "SparseSet::GetIndex > Sparse set doesn't contains this value" );

            return m_Sparse[element];
        }

        IndexType GetElement( IndexType index ) const
        {
            SM_ASSERT_MSG( index < m_Dense.GetItemCount(), "SparseSet::GetElement > Index out of range" );

            return m_Dense[index];
        }

        Count GetItemCount() const
        {
            return m_Dense.GetItemCount();
        }

        bool IsEmpty() const
        {
            return m_Dense.IsEmpty();
        }

      public:
        Vector< IndexType > m_Sparse{};
        Vector< IndexType > m_Dense{};
    };
}
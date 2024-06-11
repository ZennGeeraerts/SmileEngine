/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <vector>
#include <numeric>

namespace smile::primitive
{
    template < typename IndexType >
    class SparseSet final
    {
        static_assert( std::is_unsigned< IndexType >::value, "SparseSet can only contain unsigned integers" );

      public:
        using Iterator = typename std::vector< IndexType >::iterator;
        using ConstIterator = typename std::vector< IndexType >::const_iterator;

        SparseSet() = default;

        ~SparseSet()
        {
            Clear();
        }

        IndexType Insert( IndexType element )
        {
            SM_ASSERT( !Contains( element ), "SparseSet::Insert > Sparse set already contains this value" );

            const auto pos = static_cast< IndexType >( m_Dense.size() );
            m_Dense.push_back( element );

            if ( element >= m_Sparse.size() )
                m_Sparse.resize( element + 1, std::numeric_limits< IndexType >::max() );

            m_Sparse[element] = pos;

            return m_Dense.size() - 1;
        }

        IndexType Erase( IndexType element )
        {
            SM_ASSERT( Contains( element ), "SparseSet::Erase > Sparse set doesn't contain this value" );

            IndexType deadIndex = m_Sparse[element];

            const IndexType last = m_Dense.back();
            std::swap( m_Dense.back(), m_Dense[deadIndex] );
            std::swap( m_Sparse[last], m_Sparse[element] );
            m_Dense.pop_back();

            m_Sparse[element] = std::numeric_limits< IndexType >::max();

            return deadIndex;
        }

        bool Contains( IndexType element ) const
        {
            return ( element < m_Sparse.size() ) && ( m_Sparse[element] < m_Dense.size() ) &&
                   ( m_Sparse[element] != std::numeric_limits< IndexType >::max() );
        }

        void Clear()
        {
            m_Sparse.clear();
            m_Dense.clear();
        }

        void Swap( IndexType lhs, IndexType rhs )
        {
            SM_ASSERT( Contains( lhs ) && Contains( rhs ), "SparseSet::Swap > Sparse set doesn't contains this value" );

            if ( lhs == rhs )
                return;

            std::swap( m_Dense[m_Sparse[lhs]], m_Dense[m_Sparse[rhs]] );
            std::swap( m_Sparse[lhs], m_Sparse[rhs] );
        }

        template < typename Compare >
        void Sort( Compare compare )
        {
            std::vector< IndexType > copy( m_Dense.size() );
            std::iota( copy.begin(), copy.end(), IndexType{} );

            std::sort( copy.begin(),
                copy.end(),
                [this, compare = std::move( compare )]( const IndexType lhs, const IndexType rhs )
                { return compare( m_Dense[lhs], m_Dense[rhs] ); } );

            for ( IndexType pos{}; pos < copy.size(); ++pos )
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
            SM_ASSERT( Contains( element ), "SparseSet::GetIndex > Sparse set doesn't contains this value" );

            return m_Sparse[element];
        }

        IndexType GetElement( IndexType index ) const
        {
            SM_ASSERT( index < m_Dense.size(), "SparseSet::GetElement > Index out of range" );

            return m_Dense[index];
        }

        size_t GetItemCount() const
        {
            return m_Dense.size();
        }

        bool IsEmpty() const
        {
            return m_Dense.empty();
        }

      public:
        std::vector< IndexType > m_Sparse{};
        std::vector< IndexType > m_Dense{};
    };
}
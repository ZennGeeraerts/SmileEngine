#pragma once

#include <vector>

namespace smile::stl
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

        void Swap( IndexType element1, IndexType element2 )
        {
            SM_ASSERT( Contains( element1 ) && Contains( element2 ),
                "SparseSet::Swap > Sparse set doesn't contains this value" );

            if ( element1 == element2 )
                return;

            std::swap( m_Dense[m_Sparse[element1]], m_Dense[m_Sparse[element2]] );
            std::swap( m_Sparse[element1], m_Sparse[element2] );
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

      private:
        std::vector< IndexType > m_Sparse{};
        std::vector< IndexType > m_Dense{};
    };
}
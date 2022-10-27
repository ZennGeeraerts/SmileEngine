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
            clear();
        }

        IndexType insert( IndexType element )
        {
            SM_ASSERT( !contains( element ), "SparseSet::insert > Sparse set already contains this value" );

            const auto pos = dense.size();
            dense.push_back( element );

            if ( element >= sparse.size() )
                sparse.resize( element + 1, std::numeric_limits< IndexType >::max() );

            sparse[element] = pos;

            return dense.size() - 1;
        }

        IndexType erase( IndexType element )
        {
            SM_ASSERT( contains( element ), "SparseSet::erase > Sparse set doesn't contain this value" );

            IndexType dead_index = sparse[element];

            const IndexType last = dense.back();
            std::swap( dense.back(), dense[dead_index] );
            std::swap( sparse[last], sparse[element] );
            dense.pop_back();

            sparse[element] = std::numeric_limits< IndexType >::max();

            return dead_index;
        }

        bool contains( IndexType element ) const
        {
            return ( element < sparse.size() ) && ( sparse[element] < dense.size() ) &&
                   ( sparse[element] != std::numeric_limits< IndexType >::max() );
        }

        void clear()
        {
            sparse.clear();
            dense.clear();
        }

        void swap( IndexType element1, IndexType element2 )
        {
            SM_ASSERT( contains( element1 ) && contains( element2 ),
                "SparseSet::swap > Sparse set doesn't contains this value" );

            if ( element1 == element2 )
                return;

            std::swap( dense[sparse[element1]], dense[sparse[element2]] );
            std::swap( sparse[element1], sparse[element2] );
        }

        Iterator begin()
        {
            return dense.begin();
        }

        Iterator end()
        {
            return dense.end();
        }

        ConstIterator begin() const
        {
            return dense.begin();
        }

        ConstIterator end() const
        {
            return dense.end();
        }

        IndexType getIndex( IndexType element ) const
        {
            SM_ASSERT( contains( element ), "SparseSet::getIndex > Sparse set doesn't contains this value" );

            return sparse[element];
        }

        IndexType getElement( IndexType index ) const
        {
            SM_ASSERT( index < dense.size(), "SparseSet::getElement > Index out of range" );

            return dense[index];
        }

        size_t getItemCount() const
        {
            return dense.size();
        }

        bool isEmpty() const
        {
            return dense.empty();
        }

      private:
        std::vector< IndexType > sparse{};
        std::vector< IndexType > dense{};
    };
}
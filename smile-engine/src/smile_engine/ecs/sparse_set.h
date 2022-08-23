#pragma once

#include "entity_handle.h"

#include <vector>

namespace smile::ecs
{
    template < typename Type >
    class SparseSet final
    {
        static_assert( std::is_unsigned< Type >::value, "SparseSet can only contain unsigned integers" );

      public:
        using Iterator = typename std::vector< Type >::iterator;
        using ConstIterator = typename std::vector< Type >::const_iterator;

        SparseSet() = default;

        ~SparseSet()
        {
            clear();
        }

        Type insert( const Type &element )
        {
            SM_ASSERT( !contains( element ), "SparseSet::insert > Sparse set already contains this value" );

            const auto pos = dense.size();
            dense.push_back( element );

            if ( element >= sparse.size() )
                sparse.resize( element + 1, nullHandle.index );

            sparse[element] = pos;

            return dense.size() - 1;
        }

        Type erase( const Type &element )
        {
            SM_ASSERT( contains( element ), "SparseSet::erase > Sparse set doesn't contain this value" );

            Type dead_index = sparse[element];

            const Type last = dense.back();
            std::swap( dense.back(), dense[dead_index] );
            std::swap( sparse[last], sparse[element] );
            dense.pop_back();

            sparse[element] = nullHandle.index;

            return dead_index;
        }

        bool contains( const Type &element )
        {
            return ( element < sparse.size() ) && ( sparse[element] < dense.size() ) &&
                   ( dense[sparse[element]] != nullHandle );
        }

        void clear()
        {
            sparse.clear();
            dense.clear();
        }

        ConstIterator begin() const
        {
            return dense.begin();
        }

        ConstIterator end() const
        {
            return dense.begin() + dense.size();
        }

        const Type &getIndex( const Type &element ) const
        {
            //SM_ASSERT( contains( element ), "SparseSet::getIndex > Sparse set doesn't contains this value" );

            return sparse[element];
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
        std::vector< Type > sparse{};
        std::vector< Type > dense{};
    };
}
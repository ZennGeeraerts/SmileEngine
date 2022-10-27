#pragma once

#include <algorithm>

namespace smile::ecs
{
    class View final
    {
      public:
        struct Iterator final
        {
            Iterator( EntityHandleManager &handle_manager,
                SparseSetType::ConstIterator it,
                const std::vector< SparseSetType * > &pools )
                : handleManager{ handle_manager }, it{ it }, pools{ pools }
            {
            }

            EntityHandleType operator*() const
            {
                return handleManager.getEntityHandle( *it );
            }
            bool operator==( const Iterator &other ) const
            {
                return it == other.it || ( *it ) == handleManager.getEntityCount();
            }
            bool operator!=( const Iterator &other ) const
            {
                return it != other.it && ( *it ) != handleManager.getEntityCount();
            }

            Iterator &operator++()
            {
                ++it;

                SparseSetType *min_pool = *std::min_element( std::begin( pools ),
                    std::end( pools ),
                    []( SparseSetType *lhs, SparseSetType *rhs )
                    { return lhs->getItemCount() < rhs->getItemCount(); } );

                if ( it == min_pool->end() )
                    return *this;

                SparseSetType::ConstIterator old_it;
                do
                {
                    old_it = it;

                    for ( auto pool : pools )
                    {
                        if ( pool == min_pool )
                            continue;

                        if ( !pool->contains( *it ) )
                        {
                            ++it;
                            break;
                        }
                    }
                } while ( ( it != old_it ) && ( handleManager.getEntityHandle( *it ).isValid() ) );

                return *this;
            }

            EntityHandleManager &handleManager;
            SparseSetType::ConstIterator it;
            std::vector< SparseSetType * > pools;
        };

      public:
        View( EntityHandleManager &handle_manager, const std::vector< ComponentInterface * > &components )
            : handleManager{ handle_manager }
        {
            for ( auto component : components )
            {
                if ( component )
                    pools.push_back( &component->sparseSet );
            }
        }

        const Iterator begin() const
        {
            if ( !pools.empty() )
            {
                SparseSetType *min_pool = *std::min_element( std::begin( pools ),
                    std::end( pools ),
                    []( SparseSetType *lhs, SparseSetType *rhs )
                    { return lhs->getItemCount() < rhs->getItemCount(); } );

                return Iterator{ handleManager, min_pool->begin(), pools };
            }
            else
            {
                return Iterator{ handleManager, SparseSetType::ConstIterator{}, pools };
            }
        }

        const Iterator end() const
        {
            if ( !pools.empty() )
            {
                SparseSetType *min_pool = *std::min_element( std::begin( pools ),
                    std::end( pools ),
                    []( SparseSetType *lhs, SparseSetType *rhs )
                    { return lhs->getItemCount() < rhs->getItemCount(); } );

                return Iterator{ handleManager, min_pool->end(), pools };
            }
            else
            {
                return Iterator{ handleManager, SparseSetType::ConstIterator{}, pools };
            }
        }

      private:
        EntityHandleManager &handleManager;
        std::vector< SparseSetType * > pools{};
    };
}
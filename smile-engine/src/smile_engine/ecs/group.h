#pragma once

#include <algorithm>

namespace smile::ecs
{
    class Group final
    {
      public:
        struct Iterator final
        {
            Iterator( EntityHandleManager &handle_manager, SparseSetType::ConstIterator it )
                : handleManager{ handle_manager }, it{ it }
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
                return *this;
            }

            EntityHandleManager &handleManager;
            SparseSetType::ConstIterator it;
        };

      public:
        Group( EntityHandleManager &handle_manager,
            const std::vector< ComponentInterface * > &owned,
            const std::vector< ComponentInterface * > &get )
            : handleManager{ handle_manager }
        {
            std::vector< SparseSetType * > pools{};

            for ( auto component : owned )
            {
                if ( component )
                {
                    pools.push_back( &component->sparseSet );
                    ownedPools.push_back( component );
                }
            }

            for ( auto component : get )
            {
                if ( component )
                {
                    pools.push_back( &component->sparseSet );
                    getPools.push_back( component );
                }
            }

            if ( pools.empty() )
                return;

            SparseSetType *min_pool = *std::min_element( std::begin( pools ),
                std::end( pools ),
                []( SparseSetType *lhs, SparseSetType *rhs )
                { return lhs->getItemCount() < rhs->getItemCount(); } );

            for ( Uint32 i{}; i < min_pool->getItemCount(); ++i )
            {
                auto entity = min_pool->getElement( i );
                bool match = true;

                for ( auto pool : pools )
                {
                    if ( pool == min_pool )
                        continue;

                    if ( !pool->contains( entity ) )
                    {
                        match = false;
                        break;
                    }
                }

                if ( match )
                {
                    for ( auto component : ownedPools )
                    {
                        IndexType index = component->sparseSet.getIndex( entity );
                        component->sparseSet.swap( component->sparseSet.getElement( i ), entity );
                        component->componentStorage->swap( i, index );
                    }

                    ++endIndex;
                }
            }
        }

        void addEntity( IndexType entity_index )
        {
            if ( hasEntity( entity_index ) )
            {
                for ( auto component : ownedPools )
                {
                    IndexType index = component->sparseSet.getIndex( entity_index );
                    component->sparseSet.swap( component->sparseSet.getElement( endIndex ), entity_index );
                    component->componentStorage->swap( endIndex, index );
                }

                ++endIndex;
            }
        }

        void remove( IndexType entity_index )
        {
            if ( hasEntity( entity_index ) )
            {
                for ( auto component : ownedPools )
                {
                    IndexType index = component->sparseSet.getIndex( entity_index );
                    component->sparseSet.swap( component->sparseSet.getElement( endIndex - 1 ), entity_index );
                    component->componentStorage->swap( endIndex - 1, index );
                }

                --endIndex;
            }
        }

        bool hasComponent( ComponentInterface *component ) const
        {
            return ( std::find( ownedPools.begin(), ownedPools.end(), component ) != ownedPools.end() ) ||
                   ( std::find( getPools.begin(), getPools.end(), component ) != getPools.end() );
        }

        bool hasEntity( IndexType entity_index ) const
        {
            std::vector< SparseSetType * > pools{};

            for ( auto component : ownedPools )
                pools.push_back( &component->sparseSet );
            for ( auto component : getPools )
                pools.push_back( &component->sparseSet );

            for ( auto pool : pools )
            {
                if ( !pool->contains( entity_index ) )
                    return false;
            }

            return true;
        }

        Iterator begin() const
        {
            if ( !ownedPools.empty() )
                return Iterator{ handleManager, ( *ownedPools.begin() )->sparseSet.begin() };
            else
                return Iterator{ handleManager, SparseSetType::ConstIterator{} };
        }

        Iterator end() const
        {
            if ( !ownedPools.empty() )
                return Iterator{ handleManager, ( *ownedPools.begin() )->sparseSet.begin() + endIndex };
            else
                return Iterator{ handleManager, SparseSetType::ConstIterator{} };
        }

        const std::vector< ComponentInterface * > &getOwnedComponents() const
        {
            return ownedPools;
        }
        const std::vector< ComponentInterface * > &getGetComponents() const
        {
            return getPools;
        }

      private:
        EntityHandleManager &handleManager;

        std::vector< ComponentInterface * > ownedPools{};
        std::vector< ComponentInterface * > getPools{};

        IndexType endIndex{ 0 };
    };
}
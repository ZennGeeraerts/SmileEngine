/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::primitive
{
    template < typename CollectionType >
    class BackInserter final
    {
      public:
        using Item = typename CollectionType::Item;

        struct Inserter
        {
            Inserter &operator=( const Item &value )
            {
                pCollection->PushBack( value );

                return *this;
            }

            Inserter &operator=( Item &&value )
            {
                pCollection->EmplaceBack( std::move( value ) );

                return *this;
            }

            CollectionType *pCollection;
        };

        BackInserter( CollectionType &pCollection ) noexcept : pCollection{ pCollection }
        {
        }

        ~BackInserter() noexcept = default;

        Inserter operator*()
        {
            return { pCollection };
        }

        BackInserter &operator++()
        {
            return *this;
        }

        BackInserter &operator++( int )
        {
            return *this;
        }

        CollectionType *pCollection;
    };
}

namespace std
{
    template < class CollectionType >
    struct iterator_traits< typename smile::primitive::BackInserter< CollectionType > >
    {
        using value_type = typename smile::primitive::BackInserter< CollectionType >::Item;
        using pointer = typename smile::primitive::BackInserter< CollectionType >::Item *;
        using reference = typename smile::smileprimitive::BackInserter< CollectionType >::Item &;
        using difference_type = Uint64;
        using iterator_category = std::output_iterator_tag;
    };
}
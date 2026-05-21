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
            Inserter &operator=( const Item &value ) noexcept
            {
                pCollection->PushBack( value );

                return *this;
            }

            Inserter &operator=( Item &&value ) noexcept
            {
                pCollection->EmplaceBack( std::move( value ) );

                return *this;
            }

            CollectionType *pCollection;
        };

        explicit BackInserter( CollectionType &collection ) noexcept : pCollection{ &collection }
        {
        }

        ~BackInserter() noexcept = default;

        Inserter operator*() noexcept
        {
            return { pCollection };
        }

        BackInserter &operator++() noexcept
        {
            return *this;
        }

        BackInserter &operator++( int ) noexcept
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
        using reference = typename smile::primitive::BackInserter< CollectionType >::Item &;
        using difference_type = smile::Uint64;
        using iterator_category = std::output_iterator_tag;
    };
}
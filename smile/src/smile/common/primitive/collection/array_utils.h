/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

#include <algorithm>
#include <optional>

namespace smile::primitive::array
{
    template < typename ArrayType, typename ItemType >
    inline bool HasItem( const ArrayType &array, const ItemType &item )
    {
        auto iteratorEnd = std::end( array );
        return std::find( std::begin( array ), iteratorEnd, item ) != iteratorEnd;
    }

    template < typename ArrayType, typename ItemType >
    std::optional< Index > FindItemIndex( const ArrayType &array, const ItemType &item )
    {
        auto iteratorEnd = std::end( array );
        auto iterator = std::find( std::begin( array ), iteratorEnd, item );
        if ( iterator != iteratorEnd )
        {
            const Index index = iterator - array;
            return { index };
        }

        return {};
    }
}
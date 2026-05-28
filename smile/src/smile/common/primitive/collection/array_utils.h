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
    template < typename ArrayType >
    concept HasBeginEnd = requires( ArrayType &array ) {
        { std::begin( array ) };
        { std::end( array ) };
    };

    template < HasBeginEnd ArrayType, typename ItemType >
    inline bool HasItem( const ArrayType &array, const ItemType &item )
    {
        auto iteratorEnd = std::end( array );
        return std::find( std::begin( array ), iteratorEnd, item ) != iteratorEnd;
    }

    template < HasBeginEnd ArrayType, typename ItemType >
    inline std::optional< Index > FindItemIndex( const ArrayType &array, const ItemType &item )
    {
        auto iteratorBegin = std::begin( array );
        auto iteratorEnd = std::end( array );
        auto iterator = std::find( iteratorBegin, iteratorEnd, item );
        if ( iterator != iteratorEnd )
        {
            const Index index = iterator - iteratorBegin;
            return { index };
        }

        return std::nullopt;
    }

    template < HasBeginEnd ArrayType >
    inline bool IsEqual( const ArrayType &first, const ArrayType &second )
    {
        using std::begin;
        using std::end;

        return std::equal( begin( first ), end( first ), begin( second ), end( second ) );
    }

    template < typename ArrayType >
    concept HasEraseAtIndex = requires( ArrayType &array ) {
        typename ArrayType::Item;
        { array.EraseAtIndex( Index{} ) };
    };

    template < HasEraseAtIndex ArrayType >
    inline bool EraseIfExists( ArrayType &array, const typename ArrayType::Item &item )
    {
        std::optional< Index > index = FindItemIndex( array, item );

        if ( !index.has_value() )
            return false;

        array.EraseAtIndex( index.value() );
        return true;
    }

    template < HasBeginEnd ArrayType, typename Item >
    inline auto GetLowerBoundIndex( const ArrayType &array, const Item &item )
    {
        auto beginIt = std::begin( array );

        auto iterator = std::lower_bound( beginIt, std::end( array ), item );

        return iterator - beginIt;
    }

    template < HasBeginEnd ArrayType, typename Item, typename Predicate >
    inline auto GetLowerBoundIndex( const ArrayType &array, const Item &item, Predicate predicate )
    {
        auto beginIt = std::begin( array );

        auto iterator = std::lower_bound( beginIt, std::end( array ), item, predicate );

        return iterator - beginIt;
    }

    template < HasBeginEnd ArrayType, typename Item >
    inline auto GetUpperBoundIndex( const ArrayType &array, const Item &item )
    {
        auto beginIt = std::begin( array );

        auto iterator = std::upper_bound( beginIt, std::end( array ), item );

        return iterator - beginIt;
    }

    template < HasBeginEnd ArrayType, typename Item, typename Predicate >
    inline auto GetUpperBoundIndex( const ArrayType &array, const Item &item, Predicate predicate )
    {
        auto beginIt = std::begin( array );

        auto iterator = std::upper_bound( beginIt, std::end( array ), item, predicate );

        return iterator - beginIt;
    }
}
/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

#include <type_traits>
#include <limits>

namespace smile::foundation
{
#if SM_C_DEBUG
    namespace detail
    {
        enum class NumericType
        {
            Float,
            SignedInt,
            UnsignedInt
        };

        template < typename Type >
        struct GetNumericType;

        template <>
        struct GetNumericType< float >
        {
            constexpr static NumericType s_Value = NumericType::Float;
        };

        template <>
        struct GetNumericType< double >
        {
            constexpr static NumericType s_Value = NumericType::Float;
        };

        template < typename Type >
        struct GetNumericType
        {
            constexpr static NumericType s_Value =
                std::is_signed_v< Type > ? NumericType::SignedInt : NumericType::UnsignedInt;
        };

        template < typename Converted,
            typename Initial,
            NumericType ConvertedType = GetNumericType< Converted >::s_Value,
            NumericType InitialType = GetNumericType< Initial >::s_Value,
            bool IsUpcast = sizeof( Converted ) >= sizeof( Initial ) >
        struct NumericConverter;

        template < typename Converted, typename Initial, NumericType Type >
        struct NumericConverter< Converted, Initial, Type, Type, true >
        {
            static bool HasValidConversion( const Initial )
            {
                return true;
            }
        };

        template < typename Converted, typename Initial >
        struct NumericConverter< Converted, Initial, NumericType::UnsignedInt, NumericType::UnsignedInt, false >
        {
            static bool HasValidConversion( const Initial value )
            {
                return value <= Initial( std::numeric_limits< Converted >::max() );
            }
        };

        template < typename Converted, typename Initial, bool Dummy >
        struct NumericConverter< Converted, Initial, NumericType::SignedInt, NumericType::UnsignedInt, Dummy >
        {
            static bool HasValidConversion( const Initial value )
            {
                return value <= Initial( std::numeric_limits< Converted >::max() );
            }
        };

        template < typename Converted, typename Initial, bool Dummy >
        struct NumericConverter< Converted, Initial, NumericType::UnsignedInt, NumericType::SignedInt, Dummy >
        {
            static bool HasValidConversion( const Initial value )
            {
                return value >= 0 && Int64( value ) <= Int64( std::numeric_limits< Converted >::max() );
            }
        };

        template < typename Converted, typename Initial >
        struct NumericConverter< Converted, Initial, NumericType::UnsignedInt, NumericType::SignedInt, false >
        {
            static bool HasValidConversion( const Initial value )
            {
                return static_cast< Converted >( value ) == value;
            }
        };

        template < typename Converted, typename Initial, bool Dummy >
        struct NumericConverter< Converted, Initial, NumericType::SignedInt, NumericType::Float, Dummy >
        {
            static bool HasValidConversion( const Initial value )
            {
                return static_cast< Converted >( value ) == value;
            }
        };

        template < typename Converted, typename Initial, bool Dummy >
        struct NumericConverter< Converted, Initial, NumericType::Float, NumericType::SignedInt, Dummy >
        {
            static bool HasValidConversion( const Initial value )
            {
                return static_cast< Converted >( value ) == value;
            }
        };
    }
#endif

    template < typename Converted, typename Initial >
    constexpr Converted NumericCast( const Initial value )
    {
#if SM_C_DEBUG
        SM_ASSERT( ( detail::NumericConverter< Converted, Initial >::HasValidConversion( value ) ) );
#endif
        return static_cast< Converted >( value );
    }
}
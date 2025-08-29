/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::math
{
    template < Uint8 Size, typename Type >
    struct Vector final
    {
        inline Type operator[]( Uint8 index ) const
        {
            SM_ASSERT_MSG( index < Size, "Vector > index of Vector [] operator is out of bounds!" );
            return Data[index];
        }

        inline Type &operator[]( Uint8 index )
        {
            SM_ASSERT_MSG( index < Size, "Vector > index of Vector [] operator is out of bounds!" );
            return Data[index];
        }

        Type Data[Size];
    };

    template < Uint8 Size, typename Type >
    inline Type MagnitudeSqr( const Vector< Size, Type > &vector )
    {
        return DotProduct( vector, vector );
    }

    template < Uint8 Size, typename Type >
    inline Type Magnitude( const Vector< Size, Type > &vector )
    {
        return static_cast< Type >( SquareRoot( MagnitudeSqr( vector ) ) );
    }

    template < int Size, typename Type >
    inline Type Normalize( Vector< Size, Type > &vector )
    {
        SM_ASSERT_MSG( !IsZeroVector( vector ), "Normalize > Vector is zero vector" );

        const Type magnitude = Magnitude( vector );
        vector /= magnitude;

        return magnitude;
    }

    template < Uint8 Size, typename Type >
    inline Vector< Size, Type > GetNormalized( const Vector< Size, Type > &vector )
    {
        SM_ASSERT_MSG( !IsZeroVector( vector ), "GetNormalized > Vector is zero vector" );

        const Type magnitude = Magnitude( vector );
        return Vector{ vector / magnitude };
    }

    template < Uint8 Size, typename Type >
    inline Vector< Size, Type >
    Lerp( const Vector< Size, Type > &v1, const Vector< Size, Type > &v2, const float percentage )
    {
        return v2 + ( ( v2 - v1 ) * static_cast< Type >( percentage ) );
    }
}
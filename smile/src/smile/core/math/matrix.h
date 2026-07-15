/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        matrix.h
 * @author      Zenn Geeraerts
 * @created		3 February 2026
 * @brief       Matrix base template
 */
#pragma once

#include "math_utilities.h"

namespace smile::math
{
    template < Uint8 N, Uint8 M, Numeric Type >
    class Matrix final
    {
      public:
        constexpr Matrix() noexcept = default;
        constexpr Matrix( const Matrix & ) noexcept = default;
        constexpr Matrix( Matrix && ) noexcept = default;
        constexpr Matrix &operator=( const Matrix & ) noexcept = default;
        constexpr Matrix &operator=( Matrix && ) noexcept = default;
        ~Matrix() noexcept = default;

        Type operator()( Uint8 row, Uint8 column ) const
        {
            SM_ASSERT_MSG( row < M && column < N, "indices of Matrix () const operator are out of bounds" );

            return Data[row][column];
        }

        Type &operator()( Uint8 row, Uint8 column )
        {
            SM_ASSERT_MSG( row < M && column < N, "indices of Matrix () operator are out of bounds" );

            return Data[row][column];
        }

        const Vector< N, Type > &operator[]( Uint8 row ) const
        {
            SM_ASSERT_MSG( row < N, "index of Matrix [] operator is out of bounds" );

            return *reinterpret_cast< const Vector< N, Type > * >( Data[row] );
        }

        Vector< N, Type > &operator[]( Uint8 row )
        {
            SM_ASSERT_MSG( row < N, "index of Matrix [] operator is out of bounds" );

            return *reinterpret_cast< Vector< N, Type > * >( Data[row] );
        }

        Vector< N, Type > GetRow( const Uint8 row ) const
        {
            SM_ASSERT_MSG( row < N, "index of GetRow is out of bounds" );

            return *reinterpret_cast< const Vector< N, Type > * >( Data[row] );
        }

        Type Data[N][M];
    };
}
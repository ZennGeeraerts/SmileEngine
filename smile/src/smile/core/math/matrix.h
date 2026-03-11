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
    //========================= LAYOUT ==========================
    //------ LAYOUT IN MEMORY ---------- "LAYOUT" FOR USAGE -----
    // |xx|	 c0	 |	 c1	 |	c2	|	c3	|    |xx|	 c0	 |	 c1	 |	c2	|	c3	|
    // ---------------------------------------------------------------------------
    // |r0|  a.x |  a.y	 |  a.z	|  a.w  |	 |r0|   a.x	 |  b.x	 |  c.x	|  d.x	|
    // |r1|	 b.x |  b.y	 |  b.z	|  b.w  |	 |r1|	a.y	 |  b.y	 |  c.y	|  d.y	|
    // |r2|	 c.x |  c.y	 |  c.z	|  c.w  |	 |r2|	a.z	 |  b.z	 |  c.z	|  d.z	|
    // |r3|	 d.x |  d.y	 |  d.z	|  d.w	|	 |r3|	a.w	 |  b.w	 |  c.w	|  d.w	|
    // ----------------------------------------------------------
    // So access indices are reversed --> operator( row, column ), example usage:
    // (row 1, column 2) we expect value c.y, this is referenced in memory as [2][1] == row 2, column 1,
    // which is also c.y!

    template < Uint8 N, Uint8 M, Numeric Type >
    struct Matrix final
    {
        Type Data[N][M];

        inline Type operator()( Uint8 row, Uint8 column ) const
        {
            SM_ASSERT_MSG( row < M && column < N, "indices of Matrix () const operator are out of bounds" );

            return Data[column][row];
        }
        
        inline Type &operator()( Uint8 row, Uint8 column )
        {
            SM_ASSERT_MSG( row < M && column < N, "indices of Matrix () const operator are out of bounds" );

            return Data[column][row];
        }

        inline const Vector< N, Type > &operator[]( Uint8 column ) const
        {
            SM_ASSERT_MSG( column < N, "index of Matrix [] operator is out of bounds" );

            return *reinterpret_cast< Vector< N, Type > >( Data[column] );
        }

        inline Vector< N, Type > &operator[]( Uint8 column )
        {
            SM_ASSERT_MSG( column < N, "index of Matrix [] operator is out of bounds" );

            return *reinterpret_cast< Vector< N, Type > >( Data[column] );
        }
    };
}
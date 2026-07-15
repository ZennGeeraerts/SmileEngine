/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        debug_renderer.cpp
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       DebugRenderer implementation
 */
#include "smpch.h"
#include "debug_renderer.h"

namespace smile::graphic
{
    void DebugRenderer::DrawLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &color )
    {
        m_Vertices.PushBack( { start, color } );
        m_Vertices.PushBack( { end, color } );
    }

    void DebugRenderer::DrawLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &colorStart,
        const DirectX::XMFLOAT4 &colorEnd )
    {
        m_Vertices.PushBack( { start, colorStart } );
        m_Vertices.PushBack( { end, colorEnd } );
    }

    void DebugRenderer::DrawAABB( const DirectX::XMFLOAT3 &min,
        const DirectX::XMFLOAT3 &max,
        const DirectX::XMFLOAT4 &color )
    {
        // Bottom face
        DrawLine( { min.x, min.y, min.z }, { max.x, min.y, min.z }, color );
        DrawLine( { max.x, min.y, min.z }, { max.x, min.y, max.z }, color );
        DrawLine( { max.x, min.y, max.z }, { min.x, min.y, max.z }, color );
        DrawLine( { min.x, min.y, max.z }, { min.x, min.y, min.z }, color );
        // Top face
        DrawLine( { min.x, max.y, min.z }, { max.x, max.y, min.z }, color );
        DrawLine( { max.x, max.y, min.z }, { max.x, max.y, max.z }, color );
        DrawLine( { max.x, max.y, max.z }, { min.x, max.y, max.z }, color );
        DrawLine( { min.x, max.y, max.z }, { min.x, max.y, min.z }, color );
        // Vertical edges
        DrawLine( { min.x, min.y, min.z }, { min.x, max.y, min.z }, color );
        DrawLine( { max.x, min.y, min.z }, { max.x, max.y, min.z }, color );
        DrawLine( { max.x, min.y, max.z }, { max.x, max.y, max.z }, color );
        DrawLine( { min.x, min.y, max.z }, { min.x, max.y, max.z }, color );
    }

    void DebugRenderer::DrawSphere( const DirectX::XMFLOAT3 &center,
        float radius,
        const DirectX::XMFLOAT4 &color,
        Uint32 segments )
    {
        const float step = DirectX::XM_2PI / static_cast< float >( segments );

        auto ring = [&]( auto x, auto y )
        {
            for ( Uint32 i = 0; i < segments; ++i )
            {
                const float a0 = static_cast< float >( i ) * step;
                const float a1 = static_cast< float >( i + 1 ) * step;
                DrawLine( { center.x + x( a0 ), center.y + y( a0 ), center.z },
                    { center.x + x( a1 ), center.y + y( a1 ), center.z },
                    color );
            }
        };

        // XY ring
        ring( [&]( float a ) { return radius * std::cos( a ); }, [&]( float a ) { return radius * std::sin( a ); } );
        // XZ ring
        for ( Uint32 i = 0; i < segments; ++i )
        {
            const float a0 = static_cast< float >( i ) * step;
            const float a1 = static_cast< float >( i + 1 ) * step;
            DrawLine( { center.x + radius * std::cos( a0 ), center.y, center.z + radius * std::sin( a0 ) },
                { center.x + radius * std::cos( a1 ), center.y, center.z + radius * std::sin( a1 ) },
                color );
        }
        // YZ ring
        for ( Uint32 i = 0; i < segments; ++i )
        {
            const float a0 = static_cast< float >( i ) * step;
            const float a1 = static_cast< float >( i + 1 ) * step;
            DrawLine( { center.x, center.y + radius * std::cos( a0 ), center.z + radius * std::sin( a0 ) },
                { center.x, center.y + radius * std::cos( a1 ), center.z + radius * std::sin( a1 ) },
                color );
        }
    }

    void DebugRenderer::Flush( DebugPassData &data )
    {
        for ( const DebugVertex &v : m_Vertices )
        {
            data.LineList.PushBack( { v.Position, v.Color } );
        }
    }

    void DebugRenderer::Reset()
    {
        m_Vertices.Clear();
    }
}

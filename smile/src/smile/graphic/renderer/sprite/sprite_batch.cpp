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
 * @file        sprite_batch.cpp
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       SpriteBatch implementation
 */
#include "smpch.h"
#include "sprite_batch.h"

#include "smile/common/foundation/numeric_cast.h"
#include "smile/graphic/renderer/render_context.h"
#include "smile/graphic/renderer/render_scene.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/shader/shader_library.h"
#include "smile/graphic/rhi/resource/buffer.h"

namespace smile::graphic
{
    // Unit quad corners in local space (x, y, z=0), CCW winding
    static constexpr DirectX::XMFLOAT3 s_QuadCorners[4] = {
        { -0.5f, -0.5f, 0.0f },
        { -0.5f, 0.5f, 0.0f },
        { 0.5f, -0.5f, 0.0f },
        { 0.5f, 0.5f, 0.0f },
    };

    static constexpr DirectX::XMFLOAT2 s_QuadUVs[4] = {
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
    };

    void SpriteBatch::Initialize( ResourceManager &resourceManager )
    {
        SM_ASSERT( !m_IsInitialized );

        // Build the shared index buffer — valid for all batches of up to s_MaxQuadsPerBatch quads
        const Count maxIndices = s_MaxQuadsPerBatch * s_IndicesPerQuad;
        primitive::Vector< Index > indices;
        indices.Reserve( maxIndices );

        for ( Index index = 0; index < s_MaxQuadsPerBatch; ++index )
        {
            const Index base = index * s_VerticesPerQuad;
            indices.PushBack( base + 0 );
            indices.PushBack( base + 1 );
            indices.PushBack( base + 2 );
            indices.PushBack( base + 2 );
            indices.PushBack( base + 1 );
            indices.PushBack( base + 3 );
        }

        m_SharedIndexBuffer = resourceManager.CreateIndexBuffer( indices.GetData(), maxIndices );

        m_VertexLayout =
            rhi::BufferLayout{ { rhi::Format::RGB32_FLOAT, "POSITION" }, { rhi::Format::RG32_FLOAT, "TEXCOORD" } };

        m_IsInitialized = true;
    }

    void SpriteBatch::ShutDown( ResourceManager &resourceManager )
    {
        resourceManager.DestroyIndexBuffer( m_SharedIndexBuffer );

        for ( auto it : m_Buckets )
        {
            Bucket &bucket = it.Value;
            if ( bucket.DynamicVB.IsValid() )
                resourceManager.DestroyVertexBuffer( bucket.DynamicVB );
        }

        m_Buckets.Clear();
        m_IsInitialized = false;
    }

    void SpriteBatch::Submit( const DirectX::XMFLOAT4X4 &worldTransform, MaterialInstance material )
    {
        SM_ASSERT( m_IsInitialized );

        const DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4( &worldTransform );

        // Lazily create bucket (no GPU resources here — VB is created/updated in Flush)
        auto it = m_Buckets.FindItemAtKey( material );
        if ( it == m_Buckets.end() )
        {
            m_Buckets[material] = Bucket{};
            it = m_Buckets.FindItemAtKey( material );
        }
        Bucket &bucket = it.GetItem();

        for ( Count i = 0; i < s_VerticesPerQuad; ++i )
        {
            SpriteVertex v;
            DirectX::XMVECTOR localPos = DirectX::XMLoadFloat3( &s_QuadCorners[i] );
            DirectX::XMVECTOR worldPos = DirectX::XMVector3Transform( localPos, world );
            DirectX::XMStoreFloat3( &v.Position, worldPos );
            v.TexCoord = s_QuadUVs[i];
            bucket.Vertices.PushBack( v );
        }
    }

    void SpriteBatch::Submit( const DirectX::XMFLOAT2 &position,
        float rotation,
        const DirectX::XMFLOAT2 &size,
        MaterialInstance material )
    {
        DirectX::XMMATRIX world = DirectX::XMMatrixScaling( size.x, size.y, 1.0f ) *
                                  DirectX::XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, rotation ) *
                                  DirectX::XMMatrixTranslation( position.x, position.y, 0.0f );
        DirectX::XMFLOAT4X4 worldTransform;
        DirectX::XMStoreFloat4x4( &worldTransform, world );
        Submit( worldTransform, material );
    }

    void SpriteBatch::Flush( RenderScene &scene, ResourceManager &resourceManager, RenderContext &context )
    {
        SM_ASSERT( m_IsInitialized );

        for ( auto it : m_Buckets )
        {
            const MaterialInstance materialInstance = it.Key;
            Bucket &bucket = it.Value;
            if ( bucket.Vertices.IsEmpty() )
                continue;

            const Count vertexCount = bucket.Vertices.GetItemCount();

            if ( !bucket.DynamicVB.IsValid() || bucket.DynamicVB.GetVertexCount() < vertexCount )
            {
                if ( bucket.DynamicVB.IsValid() )
                    resourceManager.DestroyVertexBuffer( bucket.DynamicVB );

                bucket.DynamicVB = resourceManager.CreateDynamicVertexBuffer( vertexCount, m_VertexLayout );
            }

            context.FillVertexBuffer(
                bucket.DynamicVB, static_cast< void * >( bucket.Vertices.GetData() ), vertexCount );

            const Count quadCount = vertexCount / s_VerticesPerQuad;
            const Count indexCount = quadCount * s_IndicesPerQuad;

            // Emit one renderable per bucket into the Sprite layer
            Renderable &renderable = scene.AddRenderable( SceneLayer::Sprite );
            // Identity world transform — vertices are already in world space
            DirectX::XMFLOAT4X4 identity;
            DirectX::XMStoreFloat4x4( &identity, DirectX::XMMatrixIdentity() );
            renderable.SetWorldTransform( identity );

            RenderPrimitive &prim = renderable.AddPrimitive();
            // Re-use a sub-range of the shared index buffer
            prim.SetGeometry( bucket.DynamicVB, m_SharedIndexBuffer, rhi::PrimitiveTopology::TriangleList );
            prim.SetMaterialInstance( materialInstance );
        }
    }

    void SpriteBatch::Reset()
    {
        for ( auto it : m_Buckets )
        {
            Bucket &bucket = it.Value;
            bucket.Vertices.Clear();
        }
    }

    VertexBuffer &
    SpriteBatch::GetOrCreateDynamicVB( Bucket &bucket, Count vertexCount, ResourceManager &resourceManager )
    {
        if ( !bucket.DynamicVB.IsValid() || bucket.DynamicVB.GetVertexCount() < vertexCount )
        {
            if ( bucket.DynamicVB.IsValid() )
                resourceManager.DestroyVertexBuffer( bucket.DynamicVB );
            bucket.DynamicVB = resourceManager.CreateDynamicVertexBuffer( vertexCount, m_VertexLayout );
        }
        return bucket.DynamicVB;
    }
}

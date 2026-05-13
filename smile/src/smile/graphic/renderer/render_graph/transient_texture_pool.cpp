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
 * @file        transient_texture_pool.cpp
 * @author      Zenn Geeraerts
 * @created     13 May 2026
 * @brief       Per-frame transient render-target pool for the RenderGraph
 */
#include "smpch.h"
#include "transient_texture_pool.h"

#include "smile/graphic/renderer/resource/resource_manager.h"

namespace smile::graphic
{
    TransientTexturePool::TransientTexturePool( ResourceManager &resourceManager ) noexcept
        : m_ResourceManager( resourceManager )
    {
    }

    FramebufferAttachment
    TransientTexturePool::Acquire( const Uint32 width, const Uint32 height, const rhi::Format format )
    {
        const TransientTextureKey key{ width, height, format };

        auto it = m_FreeList.FindItemAtKey( key );
        if ( it != m_FreeList.end() )
        {
            auto &bucket = it.GetItem();
            if ( !bucket.IsEmpty() )
            {
                const FramebufferAttachment attachment = bucket.GetLastItem();
                bucket.EraseAtIndex( bucket.GetLastIndex() );
                return attachment;
            }
        }

        // Nothing pooled, allocate from the resource manager
        if ( rhi::GetFormatInfo( format ).Type == rhi::FormatType::DepthStencil )
        {
            return m_ResourceManager.CreateDepthAttachment( width, height );
        }
        else
        {
            return m_ResourceManager.CreateColorAttachment( width, height );
        }
    }

    void TransientTexturePool::Release( const FramebufferAttachment &attachment )
    {
        if ( !attachment.Texture.IsValid() )
            return;

        const TransientTextureKey key{
            attachment.Texture.GetWidth(), attachment.Texture.GetHeight(), attachment.TextureFormat };

        auto it = m_FreeList.FindItemAtKey( key );
        if ( it == m_FreeList.end() )
        {
            primitive::Vector< FramebufferAttachment > bucket;
            bucket.PushBack( attachment );
            m_FreeList.Insert( key, std::move( bucket ) );
        }
        else
        {
            it.GetItem().PushBack( attachment );
        }
    }

    void TransientTexturePool::Flush()
    {
        for ( auto kv : m_FreeList )
        {
            for ( auto &attachment : kv.Value )
            {
                if ( attachment.Texture.IsValid() )
                    m_ResourceManager.DestroyTexture( attachment.Texture );
            }
        }

        m_FreeList.Clear();
    }
}

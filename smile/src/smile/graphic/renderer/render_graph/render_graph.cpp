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
 * @file        render_graph.cpp
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       Render graph implementation
 */
#include "smpch.h"
#include "render_graph.h"
#include "smile/common/foundation/numeric_cast.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/renderer/render_context.h"

namespace smile::graphic
{
    // ---- RenderGraphPassResources -----------------------------------------------

    const Texture &RenderGraphPassResources::GetTexture( RenderGraphResourceHandle handle ) const noexcept
    {
        SM_ASSERT( handle.IsValid() );
        SM_ASSERT( handle.Index < m_Resources.GetItemCount() );

        return m_Resources.GetItemAtIndex( handle.Index ).ResolvedAttachment.Texture;
    }

    const Framebuffer &RenderGraphPassResources::GetFramebuffer() const noexcept
    {
        return m_Framebuffer;
    }

    // ---- RenderGraphPassBuilder --------------------------------------------------

    RenderGraphPassBuilder::RenderGraphPassBuilder( primitive::Vector< RenderGraphResourceNode > &resources,
        primitive::Vector< RenderGraphResourceHandle > &inputs,
        primitive::Vector< RenderGraphResourceHandle > &colorOutputs,
        RenderGraphResourceHandle &depthOutput ) noexcept
        : m_Resources{ resources }, m_Inputs{ inputs }, m_ColorOutputs{ colorOutputs }, m_DepthOutput{ depthOutput }
    {
    }

    RenderGraphResourceHandle RenderGraphPassBuilder::CreateTexture( const primitive::String &name,
        const RenderGraphTextureDescriptor &desc )
    {
        // Deduplicate by name: return the existing handle if a resource with this name was already declared
        for ( Index index = 0; index < m_Resources.GetItemCount(); ++index )
        {
            if ( m_Resources.GetItemAtIndex( index ).Name == name )
                return RenderGraphResourceHandle{ index };
        }

        const Index index = m_Resources.GetItemCount();
        m_Resources.PushBack();
        auto &node = m_Resources.GetLastItem();
        node.Name = name;
        node.Desc = desc;
        return RenderGraphResourceHandle{ index };
    }

    void RenderGraphPassBuilder::ReadTexture( RenderGraphResourceHandle handle )
    {
        SM_ASSERT( handle.IsValid() );
        m_Inputs.PushBack( handle );
    }

    void RenderGraphPassBuilder::WriteColor( RenderGraphResourceHandle handle )
    {
        SM_ASSERT( handle.IsValid() );
        m_ColorOutputs.PushBack( handle );
    }

    void RenderGraphPassBuilder::WriteDepth( RenderGraphResourceHandle handle )
    {
        SM_ASSERT( handle.IsValid() );
        m_DepthOutput = handle;
    }

    // ---- RenderGraph ------------------------------------------------------------

    RenderGraph::RenderGraph( ResourceManager &resourceManager ) noexcept
        : m_ResourceManager{ resourceManager }, m_TexturePool( resourceManager )
    {
    }

    RenderGraph::~RenderGraph()
    {
        FlushPool();
    }

    void RenderGraph::Compile()
    {
        SM_ASSERT_MSG( !m_IsCompiled, "RenderGraph::Compile called on an already-compiled graph" );

        // Acquire GPU textures for every declared transient resource from the pool.
        // Pool returns an existing texture when dimensions/format match; otherwise allocates.
        for ( auto &resource : m_Resources )
        {
            resource.ResolvedAttachment =
                m_TexturePool.Acquire( resource.Desc.Width, resource.Desc.Height, resource.Desc.Format );
        }

        // Build a framebuffer for each pass from its declared color and depth outputs
        for ( auto &pass : m_Passes )
        {
            const bool hasColorOutputs = !pass.ColorOutputs.IsEmpty();
            const bool hasDepthOutput = pass.DepthOutput.IsValid();

            if ( !hasColorOutputs && !hasDepthOutput )
                continue;

            primitive::FixedVector< FramebufferAttachment, rhi::s_MaxRenderTargets > colorAttachments;
            for ( const auto &handle : pass.ColorOutputs )
            {
                SM_ASSERT( handle.IsValid() );

                colorAttachments.PushBack( m_Resources.GetItemAtIndex( handle.Index ).ResolvedAttachment );
            }

            FramebufferAttachment depthAttachment{};
            if ( hasDepthOutput )
            {
                depthAttachment = m_Resources.GetItemAtIndex( pass.DepthOutput.Index ).ResolvedAttachment;
            }

            pass.ResolvedFramebuffer = m_ResourceManager.CreateFramebuffer( colorAttachments, depthAttachment );
        }

        TopologicalSort();
        m_IsCompiled = true;
    }

    void RenderGraph::TopologicalSort()
    {
        const Count passCount = m_Passes.GetItemCount();
        const Count resourceCount = m_Resources.GetItemCount();

        // Stack-allocated temporaries: pass and resource counts are small (typically < 16) and
        // constant per frame. Using FixedVector avoids 4 heap allocations per Compile() call.
        constexpr Count maxPasses{ 16 };

        // For each resource: which pass last wrote to it? (s_InvalidIndex = no producer yet)
        primitive::FixedVector< Index, maxPasses > resourceProducer;
        for ( Index index = 0; index < resourceCount; ++index )
            resourceProducer.PushBack( s_InvalidIndex );

        // Build adjacency list and producer map in a single forward pass so that each pass's
        // reads are resolved against whoever wrote the resource BEFORE this pass, then this
        // pass's writes update the producer map for subsequent passes.
        primitive::FixedVector< Index, maxPasses > inDegree;
        inDegree.Resize( passCount ); // Resize zero-initializes all entries

        // adj[i] stores indices of passes that must run after pass i
        primitive::FixedVector< primitive::FixedVector< Index, maxPasses >, maxPasses > adj;
        for ( Index index = 0; index < passCount; ++index )
            adj.EmplaceBack();

        for ( Index index = 0; index < passCount; ++index )
        {
            const auto &pass = m_Passes.GetItemAtIndex( index );

            // Step 1: build read→write edges using the producer state from BEFORE this pass
            for ( const auto &h : pass.Inputs )
            {
                const Index producer = resourceProducer[h.Index];
                if ( producer != s_InvalidIndex && producer != index )
                {
                    adj[producer].PushBack( index );
                    ++inDegree[index];
                }
            }

            // Step 2: update producers from this pass's writes so later passes see the new version
            for ( const auto &h : pass.ColorOutputs )
                resourceProducer[h.Index] = index;
            if ( pass.DepthOutput.IsValid() )
                resourceProducer[pass.DepthOutput.Index] = index;
        }

        // Kahn's algorithm — begin with all zero-in-degree passes
        primitive::FixedVector< Index, maxPasses > queue;
        for ( Index index = 0; index < passCount; ++index )
        {
            if ( inDegree[index] == 0 )
                queue.PushBack( index );
        }

        m_SortedOrder.Clear();

        while ( !queue.IsEmpty() )
        {
            const Index current = queue[queue.GetItemCount() - 1];
            queue.PopBack();
            m_SortedOrder.PushBack( current );

            for ( const Index neighbour : adj[current] )
            {
                --inDegree[neighbour];
                if ( inDegree[neighbour] == 0 )
                    queue.PushBack( neighbour );
            }
        }

        if ( m_SortedOrder.GetItemCount() != passCount )
        {
            // Cycle detected — fall back to declaration order
            m_SortedOrder.Clear();
            for ( Index index = 0; index < passCount; ++index )
                m_SortedOrder.PushBack( index );
        }
    }

    void RenderGraph::Execute( RenderContext &context )
    {
        SM_ASSERT_MSG( m_IsCompiled, "RenderGraph::Execute called before Compile" );

        for ( const Index passIndex : m_SortedOrder )
        {
            auto &pass = m_Passes.GetItemAtIndex( passIndex );
            if ( !pass.ExecuteFn )
                continue;

            RenderGraphPassResources resources{ m_Resources, pass.ResolvedFramebuffer };
            pass.ExecuteFn( resources, context );
        }
    }

    void RenderGraph::Reset()
    {
        // Return transient textures to the pool instead of destroying them.
        // The pool holds them alive for reuse next frame.
        for ( auto &resource : m_Resources )
        {
            if ( resource.ResolvedAttachment.Texture.IsValid() )
                m_TexturePool.Release( resource.ResolvedAttachment );
        }

        // Destroy framebuffer GPU objects only — textures are already gone above.
        for ( auto &pass : m_Passes )
        {
            if ( pass.ResolvedFramebuffer.IsValid() )
                m_ResourceManager.DestroyFramebuffer( pass.ResolvedFramebuffer, false );
        }

        m_Passes.Clear();
        m_Resources.Clear();
        m_SortedOrder.Clear();
        m_IsCompiled = false;
    }

    void RenderGraph::FlushPool()
    {
        m_TexturePool.Flush();
    }
}

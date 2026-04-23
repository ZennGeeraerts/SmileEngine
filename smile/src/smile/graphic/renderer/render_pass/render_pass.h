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
 * @file        render_pass.h
 * @author      Zenn Geeraerts
 * @created     17 April 2026
 * @brief       Render pass interface, uses GPU resources
 */
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/renderer/view.h"
#include "smile/graphic/renderer/resource/frame_buffer.h"

namespace smile::graphic
{
    class RenderPass : public memory::Counted
    {
      public:
        using Ref = memory::Ref< RenderPass >;
        using ConstRef = memory::Ref< const RenderPass >;

        RenderPass() = default;
        virtual ~RenderPass() = default;

        RenderPass( const RenderPass & ) = delete;
        RenderPass( RenderPass && ) = delete;
        RenderPass &operator=( const RenderPass & ) = delete;
        RenderPass &operator=( RenderPass && ) = delete;

        virtual void Initialize() = 0;
        virtual void ShutDown() = 0;

        virtual void BeginPass( const View &view ) = 0;
        virtual void Execute( const Framebuffer &framebuffer ) = 0;
        virtual void EndPass() = 0;
    };
}
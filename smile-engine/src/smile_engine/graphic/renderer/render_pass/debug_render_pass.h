/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "render_pass.h"

namespace smile::graphic
{
	class DebugRenderPass final : public RenderPass
	{
      public:
        DebugRenderPass( ecs::ECSEngine &ecsEngine ) : RenderPass( ecsEngine )
        {
        }

        DebugRenderPass( const DebugRenderPass & ) = delete;
        DebugRenderPass( DebugRenderPass && ) = delete;
        DebugRenderPass &operator=( const DebugRenderPass & ) = delete;
        DebugRenderPass &operator=( DebugRenderPass && ) = delete;

        void OnRender() override;
        void OnRender( const EditorCamera &editorCamera ) override;
	};
}
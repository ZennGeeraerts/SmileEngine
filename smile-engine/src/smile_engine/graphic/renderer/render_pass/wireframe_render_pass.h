/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_pass.h"

namespace smile::graphic
{
    class WireframeRenderPass final : public RenderPass
    {
      public:
        WireframeRenderPass( ecs::ECSEngine &ecsEngine ) : RenderPass( ecsEngine )
        {
        }

        void OnRender() override;
        void OnRender( const EditorCamera &editorCamera ) override;
    };
}
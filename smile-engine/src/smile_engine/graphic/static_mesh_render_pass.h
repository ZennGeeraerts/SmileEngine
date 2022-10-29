#pragma once

#include "render_pass.h"

namespace Smile::Graphic
{
	class StaticMeshRenderPass final : public RenderPass
    {
     public:
        void OnRender() override;
    };
}
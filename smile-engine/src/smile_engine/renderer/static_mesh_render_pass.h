#pragma once

#include "render_pass.h"

namespace smile::renderer
{
	class StaticMeshRenderPass final : public RenderPass
    {
     public:
        void onRender() override;
    };
}
#pragma once

#include "render_pass.h"

namespace smile::graphic
{
	class StaticMeshRenderPass final : public RenderPass
    {
     public:
        void OnRender() override;
    };
}
#pragma once

#include "render_pass.h"

namespace smile
{
	class StaticMeshRenderPass final : public RenderPass
    {
     public:
        void OnRender() override;
    };
}
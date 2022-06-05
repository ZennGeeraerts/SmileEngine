#pragma once

#include <thirdparty/entt/entt.hpp>

namespace smile
{
	class RenderPass
	{
     public:
        RenderPass() = default;
        virtual ~RenderPass() = default;

		virtual void AddToScene( entt::registry registry ) = 0;
		virtual void OnRender() = 0;
	};
}
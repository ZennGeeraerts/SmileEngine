/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_api/resource/vertex_array.h"

namespace graphicslib
{
	struct GLVertexArrayObject final : public VertexArray
	{
		~GLVertexArrayObject();

		void* GetInternal() const override;

		Uint32 Internal;
	};
}
/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_api/resource/vertex_buffer.h"

namespace graphicslib
{
	struct GLVertexBuffer final : public VertexBuffer
	{
		~GLVertexBuffer();

		void* GetInternal() const override;

		Uint32 Internal = 0;
	};
}
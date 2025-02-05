/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_api/resource/index_buffer.h"

namespace smile::graphic
{
    struct GLIndexBuffer final : public IndexBuffer
    {
        ~GLIndexBuffer();

        void *GetInternal() const override;

        Uint32 Internal = 0;
    };
}
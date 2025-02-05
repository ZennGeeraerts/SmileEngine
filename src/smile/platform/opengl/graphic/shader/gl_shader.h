/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_api/shader/shader.h"

namespace smile::graphic
{
    struct GLShader final : public Shader
    {
        ~GLShader();

        void *GetData() const override;

        Uint32 Internal = 0;
    };
}
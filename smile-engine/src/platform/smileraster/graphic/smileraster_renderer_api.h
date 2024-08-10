/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/renderer_api.h"

namespace smile::graphic
{
    class SmileRasterRendererAPI final : public RendererAPI
    {
      public:
        SmileRasterRendererAPI() = default;

        void Initialize( window::Window *pWindow ) override;

      private:
        window::Window *m_pWindow = nullptr;
    };
}

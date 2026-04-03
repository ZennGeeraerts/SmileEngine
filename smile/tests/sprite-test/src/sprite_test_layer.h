/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        sprite_test_layer.h
 * @author      Zenn Geeraerts
 * @created     31 March 2026
 * @brief       Sprite test application layer
 */
#pragma once

#include "smile/core/application/layer.h"
#include "smile/core/window/events/application_event.h"

#include "smile/graphic/renderer/view.h"
#include "smile/graphic/renderer/material/material.h"

namespace smile::graphic
{
    class SpriteTestLayer final : public application::Layer
    {
      public:
        SpriteTestLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate( primitive::Timestep deltaTime ) override;
        void OnEvent( window::Event &event ) override;
        void OnImGuiRender() override;

      private:
        View m_View;
        Material::Ref m_Material;
    };
}
/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/application/layer.h"
#include "smile/core/window/events/application_event.h"

namespace smile
{
    class RHITestLayer final : public application::Layer
    {
      public:
        RHITestLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate( primitive::Timestep deltaTime ) override;
        void OnEvent( window::Event &event ) override;
        void OnImGuiRender() override;

      private:
        bool OnWindowResize( window::WindowResizeEvent &e );
    };
}
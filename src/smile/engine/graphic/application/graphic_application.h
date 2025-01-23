/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "application/application.h"
#include "engine/graphic/imgui/imgui_layer.h"

namespace smile::graphic
{
    class GraphicApplication : public application::Application
    {
      public:
        GraphicApplication( const application::ApplicationDescriptor &descriptor );
        virtual ~GraphicApplication();

        void Run() override;

       protected:
        bool OnWindowResize( window::WindowResizeEvent &e ) override;

      private:
        imgui::ImGuiLayer *m_pImGuiLayer;
    };
}
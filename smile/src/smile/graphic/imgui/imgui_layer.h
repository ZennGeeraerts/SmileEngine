/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/core/application/layer.h"

#include "smile/core/window/events/mouse_event.h"
#include "smile/core/window/events/application_event.h"
#include "smile/core/window/events/key_event.h"

namespace smile::imgui
{
    class ImGuiLayer final : public application::Layer
    {
      public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent( window::Event &event ) override;

        void Begin();
        void End();

        void SetDarkThemeColors();

      private:
        void ShutDown();

        bool OnMouseButtonPressed( window::MouseButtonPressedEvent &e );
        bool OnMouseButtonReleased( window::MouseButtonReleasedEvent &e );
        bool OnMouseMoved( window::MouseMovedEvent &e );
        bool OnMouseScrolled( window::MouseScrolledEvent &e );
        bool OnKeyPressed( window::KeyPressedEvent &e );
        bool OnKeyReleased( window::KeyReleasedEvent &e );
        bool OnKeyTyped( window::KeyTypedEvent &e );
        bool OnWindowResize( window::WindowResizeEvent &e );

      private:
        bool m_IsInitialized = false;
    };
}

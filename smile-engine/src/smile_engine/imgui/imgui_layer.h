#pragma once
#include "smile_engine/core/layer.h"

#include "smile_engine/core/events/mouse_event.h"
#include "smile_engine/core/events/application_event.h"
#include "smile_engine/core/events/key_event.h"

namespace smile::imgui
{
    class ImGuiLayer final : public Layer
    {
      public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onImGuiRender() override;
        virtual void onEvent( Event &event ) override;

        void begin();
        void end();

        void setDarkThemeColors();

      private:
        void shutDown();

        bool onMouseButtonPressed( MouseButtonPressedEvent &e );
        bool onMouseButtonReleased( MouseButtonReleasedEvent &e );
        bool onMouseMoved( MouseMovedEvent &e );
        bool onMouseScrolled( MouseScrolledEvent &e );
        bool onKeyPressed( KeyPressedEvent &e );
        bool onKeyReleased( KeyReleasedEvent &e );
        bool onKeyTyped( KeyTypedEvent &e );
        bool onWindowResize( WindowResizeEvent &e );

      private:
        bool initialized = false;
    };
}

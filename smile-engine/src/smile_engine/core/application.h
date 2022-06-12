#pragma once

#include "core.h"

#include "window.h"
#include "smile_engine/core/layer_stack.h"
#include "smile_engine/core/events/event.h"
#include "smile_engine/core/events/application_event.h"

#include "smile_engine/imgui/imgui_layer.h"

namespace smile
{
    class Application
    {
      public:
        Application( const std::string &name = "Smile Game" );
        virtual ~Application();

        void run();
        void shutDown();

        void onEvent( Event &e );

        void pushLayer( Layer *layer );
        void pushOverlay( Layer *overlay );

        inline static Application &getInstance()
        {
            return *instance;
        }
        inline Window &getWindow() const
        {
            return *window;
        }

      private:
        bool onWindowClose( WindowCloseEvent &e );
        bool onWindowResize( WindowResizeEvent &e );

      private:
        std::unique_ptr< Window > window;
        imgui::ImGuiLayer *imGuiLayer;
        bool isRunning = true;
        bool isMinimized = false;
        LayerStack layerStack;

      private:
        static Application *instance;
    };

    // To be defined in client
    Application *createGame();
}

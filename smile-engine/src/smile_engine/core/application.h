#pragma once

#include "core.h"

#include "window.h"
#include "smile_engine/core/layer_stack.h"
#include "smile_engine/core/events/event.h"
#include "smile_engine/core/events/application_event.h"

#include "smile_engine/imgui/imgui_layer.h"

namespace smile
{
    struct ApplicationCommandLineArgs final
    {
        int count{ 0 };
        char **args{ nullptr };

        const char *operator[]( int index ) const
        {
            SM_ASSERT( index < count, "ApplicationCommandLineArgs > Index out of range" );
            return args[index];
        }
    };

    struct ApplicationDescriptor final
    {
        std::string name = "Smile Game";
        std::string workingDirectory;
        ApplicationCommandLineArgs commandLineArgs;
    };

    class Application
    {
      public:
        Application( const ApplicationDescriptor &descriptor );
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
        inline const ApplicationDescriptor &getDescriptor() const 
        {
            return descriptor;
        }

      private:
        bool onWindowClose( WindowCloseEvent &e );
        bool onWindowResize( WindowResizeEvent &e );

      private:
        ApplicationDescriptor descriptor;
        std::unique_ptr< Window > window;
        imgui::ImGuiLayer *imGuiLayer;
        bool isRunning = true;
        bool isMinimized = false;
        LayerStack layerStack;

      private:
        static Application *instance;
    };

    // To be defined in client
    Application *createApplication( ApplicationCommandLineArgs command_line_args );
}

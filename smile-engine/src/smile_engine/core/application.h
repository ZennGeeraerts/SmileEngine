#pragma once

#include "core.h"

#include "window.h"
#include "smile_engine/core/layer_stack.h"
#include "smile_engine/events/event.h"
#include "smile_engine/events/application_event.h"

#include "smile_engine/ui/imgui/imgui_layer.h"

namespace smile
{
    class Application
    {
      public:
        Application( const std::string &name = "Smile Game" );
        virtual ~Application();

        void Run();
        void ShutDown();

        void OnEvent( Event &e );

        void PushLayer( Layer *pLayer );
        void PushOverlay( Layer *pOverlay );

        inline static Application &GetInstance()
        {
            return *s_pInstance;
        }
        inline Window &GetWindow() const
        {
            return *m_pWindow;
        }

      private:
        bool OnWindowClose( WindowCloseEvent &e );
        bool OnWindowResize( WindowResizeEvent &e );

      private:
        std::unique_ptr< Window > m_pWindow;
        ImGuiLayer *m_pImGuiLayer;
        bool m_bRunning = true;
        bool m_bMinimized = false;
        LayerStack m_LayerStack;

      private:
        static Application *s_pInstance;
    };

    // To be defined in client
    Application *CreateGame();
}

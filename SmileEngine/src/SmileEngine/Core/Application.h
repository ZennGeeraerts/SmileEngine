#pragma once

#include "Core.h"

#include "Window.h"
#include "SmileEngine/Core/LayerStack.h"
#include "SmileEngine/Events/Event.h"
#include "SmileEngine/Events/ApplicationEvent.h"

#include "SmileEngine/UI/ImGui/ImGuiLayer.h"

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

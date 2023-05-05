/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "core.h"

#include "window_manager.h"
#include "smile_engine/core/layer_stack.h"
#include "smile_engine/core/events/event.h"
#include "smile_engine/core/events/application_event.h"

#include "smile_engine/imgui/imgui_layer.h"

namespace smile
{
    struct ApplicationCommandLineArgs final
    {
        int Count{ 0 };
        char **Args{ nullptr };

        const char *operator[]( int index ) const
        {
            SM_ASSERT( index < Count, "ApplicationCommandLineArgs::*operator > Index out of range" );
            return Args[index];
        }
    };

    struct ApplicationDescriptor final
    {
        std::string Name = "Smile Game";
        std::string WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;
    };

    class Application
    {
      public:
        Application( const ApplicationDescriptor &descriptor );
        virtual ~Application();

        void Run();
        void ShutDown();

        void OnEvent( Event &e );

        void PushLayer( Layer *pLayer );
        void PushOverlay( Layer *pOverlay );

        inline static Application &GetInstance()
        {
            return *s_Instance;
        }
        inline Window &GetMainWindow() const
        {
            return *m_pWindowManager->GetWindow( 0 );
        }
        inline const ApplicationDescriptor &GetDescriptor() const
        {
            return m_Descriptor;
        }

      private:
        bool OnWindowClose( WindowCloseEvent &e );
        bool OnWindowResize( WindowResizeEvent &e );

      private:
        ApplicationDescriptor m_Descriptor;
        std::unique_ptr< WindowManager > m_pWindowManager;
        imgui::ImGuiLayer *m_pImGuiLayer;
        bool m_IsRunning = true;
        bool m_IsMinimized = false;
        LayerStack m_LayerStack;

      private:
        static Application *s_Instance;
    };

    // To be defined in client
    Application *CreateApplication( ApplicationCommandLineArgs commandLineArgs );
}

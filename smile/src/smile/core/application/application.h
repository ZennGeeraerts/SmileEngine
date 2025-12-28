/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/string.h"

#include "layer_stack.h"
#include "smile/core/window/window_manager.h"
#include "smile/core/window/events/event.h"
#include "smile/core/window/events/application_event.h"
#include "smile/core/fs/path.h"

namespace smile::application
{
    struct ApplicationCommandLineArgs final
    {
        int Count{ 0 };
        char **Args{ nullptr };

        const char *operator[]( int index ) const
        {
            SM_ASSERT_MSG( index < Count, "ApplicationCommandLineArgs::*operator > Index out of range" );
            return Args[index];
        }
    };

    struct ApplicationDescriptor final
    {
        primitive::String Name = "Smile Game";
        fs::Path WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;
    };

    class Application
    {
      public:
        Application( const ApplicationDescriptor &descriptor );
        virtual ~Application() = default;

        virtual void Run();
        void ShutDown();

        void OnEvent( window::Event &e );

        void PushLayer( Layer *pLayer );
        void PushOverlay( Layer *pOverlay );
        void PopLayer( Layer *pLayer );
        void PopOverLay( Layer *pLayer );

        inline static Application &GetInstance()
        {
            return *s_pInstance;
        }
        inline window::Window &GetMainWindow() const
        {
            return *m_pWindowManager->GetWindow( 0 );
        }
        inline const ApplicationDescriptor &GetDescriptor() const
        {
            return m_Descriptor;
        }

      protected:
        virtual bool OnWindowResize( window::WindowResizeEvent &e );

      private:
        bool OnWindowClose( window::WindowCloseEvent &e );

      private:
        ApplicationDescriptor m_Descriptor;

      protected:
        std::unique_ptr< window::WindowManager > m_pWindowManager;
        bool m_IsRunning = true;
        bool m_IsMinimized = false;
        LayerStack m_LayerStack;

      private:
        static Application *s_pInstance;
    };

    // To be defined in client
    Application *CreateApplication( ApplicationCommandLineArgs commandLineArgs );
}

/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "application.h"

#include "smile/core/input/input.h"

#include "timer.h"

#include <filesystem>

namespace smile::application
{
    Application *Application::s_pInstance = nullptr;

    Application::Application( const ApplicationDescriptor &descriptor ) : m_Descriptor{ descriptor }
    {
        SM_ASSERT_MSG(
            !s_pInstance, "Application::Application > There is already an instance of SmileGame, there can only be 1" );
        s_pInstance = this;

        if ( !descriptor.WorkingDirectory.empty() )
            std::filesystem::current_path( descriptor.WorkingDirectory );

        m_pWindowManager = std::unique_ptr< window::WindowManager >( window::WindowManager::Create() );
        window::Window *pMainWindow = m_pWindowManager->CreateNewWindow( window::WindowSettings{ descriptor.Name } );
        pMainWindow->SetEventCallback( SM_BIND_EVENT_FN( Application::OnEvent ) );
        pMainWindow->SetVSync( false );
    }

    void Application::PushLayer( Layer *pLayer )
    {
        m_LayerStack.PushLayer( pLayer );
    }

    void Application::PushOverlay( Layer *pOverlay )
    {
        m_LayerStack.PushOverlay( pOverlay );
    }

    void Application::PopLayer( Layer *pLayer )
    {
        m_LayerStack.PopLayer( pLayer );
    }

    void Application::PopOverLay( Layer *pLayer )
    {
        m_LayerStack.PopOverlay( pLayer );
    }

    void Application::OnEvent( window::Event &e )
    {
        window::EventDispatcher dispatcher{ e };
        dispatcher.Dispatch< window::WindowCloseEvent >( SM_BIND_EVENT_FN( Application::OnWindowClose ) );
        dispatcher.Dispatch< window::WindowResizeEvent >( SM_BIND_EVENT_FN( Application::OnWindowResize ) );

        for ( auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            ( *--it )->OnEvent( e );
            if ( e.GetIsHandled() )
                break;
        }
    }

    void Application::Run()
    {
        Timer &timer = Timer::GetInstance();
        timer.Run();

        while ( m_IsRunning )
        {
            timer.OnUpdate();
            primitive::Timestep deltaTime = timer.GetDeltaTime();

            if ( !m_IsMinimized )
            {
                for ( Layer *pLayer : m_LayerStack )
                    pLayer->OnUpdate( deltaTime );
            }

            m_pWindowManager->PollEvents();
        }
    }

    void Application::ShutDown()
    {
        m_IsRunning = false;
    }

    bool Application::OnWindowClose( window::WindowCloseEvent &e )
    {
        m_IsRunning = false;
        return true;
    }

    bool Application::OnWindowResize( window::WindowResizeEvent &e )
    {
        m_IsMinimized = ( e.GetWidth() == 0 ) || ( e.GetHeight() == 0 );
        return false;
    }
}
/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "application.h"

#include "smile/core/input/input.h"
#include "timer.h"
#include "smile/core/fs/physical_system.h"

namespace smile::application
{
    Application *Application::s_pInstance = nullptr;

    Application::Application( const ApplicationDescriptor &descriptor ) noexcept : m_Descriptor{ descriptor }
    {
        SM_ASSERT_MSG( !s_pInstance,
            "Application::Application > There is already an instance of Application, there can only be 1" );
        s_pInstance = this;
    }

    void Application::Initialize()
    {
        if ( !m_Descriptor.WorkingDirectory.IsEmpty() )
            fs::PhysicalSystem::SetCurrentDirectory( m_Descriptor.WorkingDirectory );

        m_pWindowManager = window::WindowManager::Create();

        window::Window::Ref pMainWindow =
            m_pWindowManager->CreateAppWindow( window::WindowSettings{ m_Descriptor.Name } );

        pMainWindow->SetEventCallback( SM_BIND_EVENT_FN( Application::OnEvent ) );
        pMainWindow->SetVSync( false );

        m_IsRunning = true;
    }

    void Application::ShutDown()
    {
        m_IsRunning = false;
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

            m_pWindowManager->PollEvents();

            if ( !m_IsMinimized )
            {
                for ( Layer *pLayer : m_LayerStack )
                    pLayer->OnUpdate( deltaTime );
            }
        }
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
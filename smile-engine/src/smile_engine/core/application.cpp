#include "smpch.h"
#include "application.h"

#include "logger.h"
#include "smile_engine/input/input.h"
#include "smile_engine/graphic/renderer.h"
#include "smile_engine/physics/physics_engine.h"
#include "smile_engine/scripting/script_engine.h"

#include <filesystem>

namespace smile
{
    Application *Application::s_Instance = nullptr;

    Application::Application( const ApplicationDescriptor &descriptor ) : m_Descriptor{ descriptor }
    {
        SM_ASSERT( !s_Instance, "SmileGame::SmileGame > There is already an instance of SmileGame, there can only be 1" );
        s_Instance = this;

        Logger::SetPriority( LogPriority::Trace );

        if ( !descriptor.WorkingDirectory.empty() )
            std::filesystem::current_path( descriptor.WorkingDirectory );

        m_pWindow = std::unique_ptr< Window >( Window::Create( WindowSettings{ descriptor.Name } ) );
        m_pWindow->SetEventCallback( SM_BIND_EVENT_FN( Application::OnEvent ) );
        m_pWindow->SetVSync( false );

        graphic::Renderer::Initialize();
        physics::PhysicsEngine::Initialize();
        scripting::ScriptEngine::Initialize();

        m_pImGuiLayer = new imgui::ImGuiLayer{};
        PushOverlay( m_pImGuiLayer );
    }

    Application::~Application()
    {
        scripting::ScriptEngine::ShutDown();
        physics::PhysicsEngine::ShutDown();
        graphic::Renderer::ShutDown();
    }

    void Application::PushLayer( Layer *pLayer )
    {
        m_LayerStack.PushLayer( pLayer );
    }

    void Application::PushOverlay( Layer *pOverlay )
    {
        m_LayerStack.PushOverlay( pOverlay );
    }

    void Application::OnEvent( Event &e )
    {
        EventDispatcher dispatcher{ e };
        dispatcher.Dispatch< WindowCloseEvent >( SM_BIND_EVENT_FN( Application::OnWindowClose ) );
        dispatcher.Dispatch< WindowResizeEvent >( SM_BIND_EVENT_FN( Application::OnWindowResize ) );

        for ( auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            ( *--it )->OnEvent( e );
            if ( e.m_IsHandled )
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
            Timestep deltaTime = timer.GetDeltaTime();

            if ( !m_IsMinimized )
            {
                for ( Layer *pLayer : m_LayerStack )
                    pLayer->OnUpdate( deltaTime );
            }

            m_pImGuiLayer->Begin();
            for ( Layer *pLayer : m_LayerStack )
                pLayer->OnImGuiRender();
            m_pImGuiLayer->End();

            m_pWindow->OnUpdate();
        }
    }

    void Application::ShutDown()
    {
        m_IsRunning = false;
    }

    bool Application::OnWindowClose( WindowCloseEvent &e )
    {
        m_IsRunning = false;
        return true;
    }

    bool Application::OnWindowResize( WindowResizeEvent &e )
    {
        m_IsMinimized = ( e.GetWidth() == 0 ) || ( e.GetHeight() == 0 );

        if ( !m_IsMinimized )
            graphic::Renderer::OnWindowResize( e.GetWidth(), e.GetHeight() );

        return false;
    }
}
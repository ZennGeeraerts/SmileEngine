/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "graphic_application.h"

#include "engine/graphic/renderer/render_engine.h"
#include "engine/graphic/renderer/render_command.h"
#include "engine/scripting/script_engine.h"

#include "application/timer.h"

namespace smile::graphic
{
    GraphicApplication::GraphicApplication( const application::ApplicationDescriptor &descriptor )
        : Application{ descriptor }
    {
        window::Window &mainWindow = GetMainWindow();
        RenderEngine::Initialize( &mainWindow );
        scripting::ScriptEngine::Initialize();

        m_pImGuiLayer = new imgui::ImGuiLayer{};
        PushOverlay( m_pImGuiLayer );
    }

    GraphicApplication::~GraphicApplication()
    {
        scripting::ScriptEngine::ShutDown();
        RenderEngine::ShutDown();
    }

    void GraphicApplication::Run()
    {
        application::Timer &timer = application::Timer::GetInstance();
        timer.Run();

        while ( m_IsRunning )
        {
            timer.OnUpdate();
            primitive::Timestep deltaTime = timer.GetDeltaTime();

            if ( !m_IsMinimized )
            {
                for ( application::Layer *pLayer : m_LayerStack )
                    pLayer->OnUpdate( deltaTime );
            }

            m_pImGuiLayer->Begin();
            for ( application::Layer *pLayer : m_LayerStack )
                pLayer->OnImGuiRender();
            m_pImGuiLayer->End();

            m_pWindowManager->PollEvents();
            RenderCommand::Present();
        }
    }

    bool GraphicApplication::OnWindowResize( window::WindowResizeEvent &e )
    {
        Application::OnWindowResize( e );

         if ( !m_IsMinimized )
            RenderEngine::OnWindowResize( e.GetWidth(), e.GetHeight() );

        return false;
    }
}
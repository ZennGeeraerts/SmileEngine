#include "smpch.h"
#include "application.h"

#include "logger.h"
#include "smile_engine/input/input.h"
#include "smile_engine/renderer/renderer.h"
#include "smile_engine/physics/physics_engine.h"

#include <filesystem>

namespace smile
{
    Application *Application::instance = nullptr;

    Application::Application( const ApplicationDescriptor &descriptor ) : descriptor{ descriptor }
    {
        SM_ASSERT( !instance, "SmileGame::SmileGame > There is already an instance of SmileGame, there can only be 1" );
        instance = this;

        Logger::setPriority( LogPriority::Trace );

        if ( !descriptor.workingDirectory.empty() )
            std::filesystem::current_path( descriptor.workingDirectory );

        window = std::unique_ptr< Window >( Window::create( WindowSettings{ descriptor.name } ) );
        window->setEventCallback( SM_BIND_EVENT_FN( Application::onEvent ) );
        window->setVSync( false );

        renderer::Renderer::initialize();
        physics::PhysicsEngine::initialize();

        imGuiLayer = new imgui::ImGuiLayer{};
        pushOverlay( imGuiLayer );
    }

    Application::~Application()
    {
        renderer::Renderer::shutDown();
        physics::PhysicsEngine::shutDown();
    }

    void Application::pushLayer( Layer *layer )
    {
        layerStack.pushLayer( layer );
    }

    void Application::pushOverlay( Layer *overlay )
    {
        layerStack.pushOverlay( overlay );
    }

    void Application::onEvent( Event &e )
    {
        EventDispatcher dispatcher{ e };
        dispatcher.dispatch< WindowCloseEvent >( SM_BIND_EVENT_FN( Application::onWindowClose ) );
        dispatcher.dispatch< WindowResizeEvent >( SM_BIND_EVENT_FN( Application::onWindowResize ) );

        for ( auto it = layerStack.end(); it != layerStack.begin(); )
        {
            ( *--it )->onEvent( e );
            if ( e.isHandled )
                break;
        }
    }

    void Application::run()
    {
        Timer &timer = Timer::getInstance();
        timer.run();

        while ( isRunning )
        {
            timer.onUpdate();
            Timestep delta_time = timer.getDeltaTime();

            if ( !isMinimized )
            {
                for ( Layer *layer : layerStack )
                    layer->onUpdate( delta_time );
            }

            imGuiLayer->begin();
            for ( Layer *layer : layerStack )
                layer->onImGuiRender();
            imGuiLayer->end();

            window->onUpdate();
        }
    }

    void Application::shutDown()
    {
        isRunning = false;
    }

    bool Application::onWindowClose( WindowCloseEvent &e )
    {
        isRunning = false;
        return true;
    }

    bool Application::onWindowResize( WindowResizeEvent &e )
    {
        isMinimized = ( e.getWidth() == 0 ) || ( e.getHeight() == 0 );

        if ( !isMinimized )
            renderer::Renderer::onWindowResize( e.getWidth(), e.getHeight() );

        return false;
    }
}
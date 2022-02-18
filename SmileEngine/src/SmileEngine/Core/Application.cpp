#include "smpch.h"
#include "Application.h"

#include "Logger.h"
#include "Input.h"
#include "SmileEngine/Renderer/Renderer.h"
#include "SmileEngine/Physics/PhysicsEngine.h"

namespace Smile
{
	Application* Application::m_pInstance = nullptr;

	Application::Application(const std::string& name)
	{
		SM_ASSERT(!m_pInstance, "SmileGame::SmileGame > There is already an instance of SmileGame, there can only be 1");
		m_pInstance = this;

		Logger::SetPriority(LogPriority::eTrace);

		m_pWindow = std::unique_ptr<Window>(Window::Create(WindowSettings{ name }));
		m_pWindow->SetEventCallback(SM_BIND_EVENT_FN(Application::OnEvent));
		m_pWindow->SetVSync(false);

		Renderer::Initialize();
		PhysicsEngine::Initialize();

		m_pImGuiLayer = new ImGuiLayer{};
		PushOverlay(m_pImGuiLayer);
	}

	Application::~Application()
	{
		delete Input::GetInstance();
		Renderer::ShutDown();
		PhysicsEngine::ShutDown();
	}

	void Application::PushLayer(Layer* pLayer)
	{
		m_LayerStack.PushLayer(pLayer);
	}

	void Application::PushOverlay(Layer* pOverlay)
	{
		m_LayerStack.PushOverlay(pOverlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher{ e };
		dispatcher.Dispatch<WindowCloseEvent>(SM_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SM_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_bHandled)
				break;
		}
	}

	void Application::Run()
	{
		Timer& timer = Timer::GetInstance();
		timer.Run();

		while (m_bRunning)
		{
			timer.OnUpdate();
			Timestep deltaTime = timer.GetDeltaTime();

			if (!m_bMinimized)
			{
				for (Layer* pLayer : m_LayerStack)
					pLayer->OnUpdate(deltaTime);
			}

			m_pImGuiLayer->Begin();
			for (Layer* pLayer : m_LayerStack)
				pLayer->OnImGuiRender();
			m_pImGuiLayer->End();

			m_pWindow->OnUpdate();
		}
	}

	void Application::ShutDown()
	{
		m_bRunning = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		m_bMinimized = (e.GetWidth() == 0) || (e.GetHeight() == 0);

		if (!m_bMinimized)
			Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
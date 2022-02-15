#include "smpch.h"
#include "SmileGame.h"

#include "Logger.h"
#include "Input.h"
#include "SmileEngine/Renderer/Renderer.h"
#include "SmileEngine/Physics/PhysicsEngine.h"

namespace Smile
{
	SmileGame* SmileGame::m_pInstance = nullptr;

	SmileGame::SmileGame(const std::string& name)
	{
		SM_ASSERT(!m_pInstance, "SmileGame::SmileGame > There is already an instance of SmileGame, there can only be 1");
		m_pInstance = this;

		Logger::SetPriority(LogPriority::eTrace);

		m_pWindow = std::unique_ptr<Window>(Window::Create(WindowSettings{ name }));
		m_pWindow->SetEventCallback(SM_BIND_EVENT_FN(SmileGame::OnEvent));
		m_pWindow->SetVSync(false);

		Renderer::Initialize();
		PhysicsEngine::Initialize();

		m_pImGuiLayer = new ImGuiLayer{};
		PushOverlay(m_pImGuiLayer);
	}

	SmileGame::~SmileGame()
	{
		delete Input::GetInstance();
		Renderer::CleanUp();
		PhysicsEngine::ShutDown();
	}

	void SmileGame::PushLayer(Layer* pLayer)
	{
		m_LayerStack.PushLayer(pLayer);
	}

	void SmileGame::PushOverlay(Layer* pOverlay)
	{
		m_LayerStack.PushOverlay(pOverlay);
	}

	void SmileGame::OnEvent(Event& e)
	{
		EventDispatcher dispatcher{ e };
		dispatcher.Dispatch<WindowCloseEvent>(SM_BIND_EVENT_FN(SmileGame::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SM_BIND_EVENT_FN(SmileGame::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_bHandled)
				break;
		}
	}

	void SmileGame::Run()
	{
		SmTime& time = SmTime::GetInstance();
		time.Run();

		while (m_bRunning)
		{
			time.OnUpdate();
			Timestep deltaTime = time.GetDeltaTime();

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

	void SmileGame::ShutDown()
	{
		m_bRunning = false;
	}

	bool SmileGame::OnWindowClose(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}

	bool SmileGame::OnWindowResize(WindowResizeEvent& e)
	{
		m_bMinimized = (e.GetWidth() == 0) || (e.GetHeight() == 0);

		if (!m_bMinimized)
			Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
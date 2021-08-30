#include "smpch.h"
#include "SmileGame.h"

#include "Logger.h"
#include "Input.h"
#include "Renderer/Renderer.h"

namespace Smile
{
	SmileGame* SmileGame::m_pInstance = nullptr;

	SmileGame::SmileGame()
		: m_bRunning{ true }
	{
		SM_ASSERT(!m_pInstance, "SmileGame::SmileGame > There is already an instance of SmileGame, there can only be 1");
		m_pInstance = this;

		Logger::SetPriority(LogPriority::eTrace);

		m_pWindow = std::unique_ptr<Window>(Window::Create());
		m_pWindow->SetEventCallback(SM_BIND_EVENT_FN(SmileGame::OnEvent));
		m_pWindow->SetVSync(false);

		m_pImGuiLayer = new ImGuiLayer{};
		PushOverlay(m_pImGuiLayer);
	}

	SmileGame::~SmileGame()
	{
		delete Input::GetInstance();
		Renderer::CleanUp();
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

			for (Layer* pLayer : m_LayerStack)
				pLayer->OnUpdate(deltaTime);

			m_pImGuiLayer->Begin();
			for (Layer* pLayer : m_LayerStack)
				pLayer->OnImGuiRender();
			m_pImGuiLayer->End();
			
			m_pWindow->OnUpdate();
		}
	}

	bool SmileGame::OnWindowClose(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}
}
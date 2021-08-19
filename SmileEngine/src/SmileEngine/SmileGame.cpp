#include "smpch.h"
#include "SmileGame.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Logger.h"
#include "SmileEngine/Renderer/RenderingContext.h"

#include "Input.h"

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

		m_pImGuiLayer = new ImGuiLayer{};
		PushOverlay(m_pImGuiLayer);
	}

	SmileGame::~SmileGame()
	{
		delete Input::GetInstance();
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

		//SM_TRACE("%s", e.ToString().c_str());

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
			RenderingContext* pRenderingContext = m_pWindow->GetRenderingContext();
			pRenderingContext->ClearBackbuffer();
			
			time.OnUpdate();

			//SM_INFO("%d", time.GetFPS());

			for (Layer* pLayer : m_LayerStack)
				pLayer->OnUpdate();

			m_pImGuiLayer->Begin();
			for (Layer* pLayer : m_LayerStack)
				pLayer->OnImGuiRender();
			m_pImGuiLayer->End();
			
			m_pWindow->OnUpdate();
			pRenderingContext->PresentBackbuffer();
		}
	}

	bool SmileGame::OnWindowClose(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}
}
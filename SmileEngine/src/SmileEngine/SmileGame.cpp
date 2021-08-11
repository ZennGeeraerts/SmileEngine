#include "smpch.h"
#include "SmileGame.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Logger.h"

namespace Smile
{

#define BIND_EVENT_FN(x) std::bind(&SmileGame::x, this, std::placeholders::_1)

	SmileGame::SmileGame()
		: m_bRunning{ true }
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	SmileGame::~SmileGame()
	{

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
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		SM_TRACE("%s", e.ToString().c_str());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_bHandled)
				break;
		}
	}

	void SmileGame::Run()
	{
		while (m_bRunning)
		{
			for (Layer* pLayer : m_LayerStack)
				pLayer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	bool SmileGame::OnWindowClose(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}
}
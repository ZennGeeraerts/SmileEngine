#pragma once

#include "Core.h"

#include "Window.h"
#include "SmileEngine/LayerStack.h"
#include "SmileEngine/Events/Event.h"
#include "SmileEngine/Events/ApplicationEvent.h"

namespace Smile
{
	class SMILE_API SmileGame
	{
	public:
		SmileGame();
		virtual ~SmileGame();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* pLayer);
		void PushOverlay(Layer* pOverlay);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_bRunning;
		LayerStack m_LayerStack;
	};

	// To be defined in client
	SmileGame* CreateGame();
}


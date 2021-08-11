#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace Smile
{
	class SMILE_API SmileGame
	{
	public:
		SmileGame();
		virtual ~SmileGame();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_bRunning;
	};

	// To be defined in client
	SmileGame* CreateGame();
}


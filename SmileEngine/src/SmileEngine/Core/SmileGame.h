#pragma once

#include "Core.h"

#include "Window.h"
#include "SmileEngine/Core/LayerStack.h"
#include "SmileEngine/Events/Event.h"
#include "SmileEngine/Events/ApplicationEvent.h"

#include "SmileEngine/ImGui/ImGuiLayer.h"

namespace Smile
{
	class SmileGame
	{
	public:
		SmileGame(const std::string& name = "Smile Game");
		virtual ~SmileGame();

		void Run();
		void ShutDown();

		void OnEvent(Event& e);

		void PushLayer(Layer* pLayer);
		void PushOverlay(Layer* pOverlay);

		inline static SmileGame& GetInstance() { return *m_pInstance; };
		inline Window& GetWindow() const { return *m_pWindow; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_pWindow;
		ImGuiLayer* m_pImGuiLayer;
		bool m_bRunning = true;
		bool m_bMinimized = false;
		LayerStack m_LayerStack;

	private:
		static SmileGame* m_pInstance;
	};

	// To be defined in client
	SmileGame* CreateGame();
}


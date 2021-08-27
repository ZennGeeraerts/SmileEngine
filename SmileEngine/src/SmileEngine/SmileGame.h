#pragma once

#include "Core.h"

#include "Window.h"
#include "SmileEngine/LayerStack.h"
#include "SmileEngine/Events/Event.h"
#include "SmileEngine/Events/ApplicationEvent.h"

#include "SmileEngine/ImGui/ImGuiLayer.h"

#include "Platform/DirectX11/DirectX11Shader.h"
#include "Platform/DirectX11/DirectX11Buffer.h"

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

		inline static SmileGame& GetInstance() { return *m_pInstance; };
		inline Window& GetWindow() const { return *m_pWindow; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_pWindow;
		ImGuiLayer* m_pImGuiLayer;
		bool m_bRunning;
		LayerStack m_LayerStack;

		static SmileGame* m_pInstance;

		std::unique_ptr<DirectX11Shader> m_pShader;
		std::unique_ptr<VertexBuffer> m_pVertexBuffer;
		std::unique_ptr<IndexBuffer> m_pIndexBuffer;
	};

	// To be defined in client
	SmileGame* CreateGame();
}


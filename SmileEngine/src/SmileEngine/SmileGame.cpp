#include "smpch.h"
#include "SmileGame.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Logger.h"

#include "SmileEngine/Renderer/RenderingContext.h" // temp
#include "Platform/DirectX11/DirectX11Context.h"
#include "Platform/DirectX11/DirectX11DataStructs.h"

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

		using namespace DirectX;
		PosColVertex vertices[3]
		{
			{ XMFLOAT3{ 0.f, 0.5f, 0.5f }, XMFLOAT3{ 1.f, 0.f, 0.f } },
			{ XMFLOAT3{ 0.5f, -0.5f, 0.5f }, XMFLOAT3{ 0.f, 1.f, 0.f } },
			{ XMFLOAT3{ -0.5f, -0.5f, 0.5f }, XMFLOAT3{ 0.f, 0.f, 1.f } }
		};
		m_pVertexBuffer.reset(VertexBuffer::Create(m_pWindow->GetRenderingContext(), vertices, 3, sizeof(PosColVertex)));

		uint32_t indices[]{ 0, 1, 2 };
		m_pIndexBuffer.reset(IndexBuffer::Create(m_pWindow->GetRenderingContext(), indices, 3));

		m_pShader.reset(new DirectX11Shader{ static_cast<DirectX11Context*>(m_pWindow->GetRenderingContext()), "../SmileProject/Resources/shaders/Color.fx" });
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
			pRenderingContext->ClearBuffer();
			
			time.OnUpdate();

			m_pVertexBuffer->Bind();
			m_pIndexBuffer->Bind();
			m_pShader->Bind();
			static_cast<DirectX11Context*>(pRenderingContext)->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			D3DX11_TECHNIQUE_DESC techDesc{};
			m_pShader->GetEffect()->GetTechniqueByIndex(0)->GetDesc(&techDesc);
			for (UINT p{}; p < techDesc.Passes; ++p)
			{
				m_pShader->GetEffect()->GetTechniqueByIndex(0)->GetPassByIndex(p)->Apply(0, static_cast<DirectX11Context*>(pRenderingContext)->GetDeviceContext());
				static_cast<DirectX11Context*>(pRenderingContext)->GetDeviceContext()->DrawIndexed(3, 0, 0);
			}

			for (Layer* pLayer : m_LayerStack)
				pLayer->OnUpdate();

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
#include "smpch.h"
#include "Renderer.h"


namespace Smile
{
	RendererSettings Renderer::m_Settings{};
	RenderCollector* Renderer::m_pRenderCollector = new RenderCollector{};

	Ref<Framebuffer> Renderer::m_pFinalSceneFramebuffer{};

	Ref<RasterizerState> Renderer::m_pWireframeRasterizerState{};

	void Renderer::Initialize()
	{
		RenderCommand::Initalize();

		DirectX::XMStoreFloat4x4(&m_pRenderCollector->ViewInverseMatrix, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_pRenderCollector->ViewProjectionMatrix, DirectX::XMMatrixIdentity());

		{
			FramebufferData fbData{};
			fbData.Attachments = { { FramebufferTextureFormat::eRGBA8, true }, FramebufferTextureFormat::eDepth, { FramebufferTextureFormat::eRGBA8, true } };
			fbData.Width = m_Settings.Width;
			fbData.Height = m_Settings.Height;
			fbData.bSwapChainTarget = false;

			m_pFinalSceneFramebuffer = Framebuffer::Create(fbData);
			m_pFinalSceneFramebuffer->SetClearColor({ DirectX::Colors::DodgerBlue.f[0], DirectX::Colors::DodgerBlue.f[1], DirectX::Colors::DodgerBlue.f[2], DirectX::Colors::DodgerBlue.f[3] });
		}
		{
			RasterizerStateData rasterizerStateData{};
			rasterizerStateData.CullMode = CullMode::eNone;
			rasterizerStateData.FillMode = FillMode::eWireFrame;
			rasterizerStateData.bDepthClipEnable = true;

			m_pWireframeRasterizerState = RasterizerState::Create(rasterizerStateData);
		}
	}

	void Renderer::ShutDown()
	{
		ClearDrawlist();
		SAFE_DELETE(m_pRenderCollector);

		RenderCommand::ShutDown();
	}

	void Renderer::SetSettings(const RendererSettings& settings)
	{
		m_Settings = settings;
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::ResizeWindow(0, 0, width, height);
	}

	void Renderer::ResizeFramebuffer(uint32_t width, uint32_t height)
	{
		m_Settings.Width = width;
		m_Settings.Height = height;

		m_pFinalSceneFramebuffer->Resize(width, height);
	}

	void Renderer::BeginScene(const Camera& camera, const DirectX::XMFLOAT4X4& cameraTransform)
	{
		auto cameraTransformMat = DirectX::XMLoadFloat4x4(&cameraTransform);
		auto projectionMatrixMat = DirectX::XMLoadFloat4x4(&camera.GetProjectionMatrix());
		auto ViewMatrixMat = DirectX::XMMatrixInverse(nullptr, cameraTransformMat);
		auto viewProjectionMatrixMat = ViewMatrixMat * projectionMatrixMat;

		DirectX::XMStoreFloat4x4(&m_pRenderCollector->ViewProjectionMatrix, viewProjectionMatrixMat);
		DirectX::XMStoreFloat4x4(&m_pRenderCollector->ViewInverseMatrix, cameraTransformMat);
	}

	void Renderer::BeginScene(const EditorCamera& editorCamera)
	{
		m_pRenderCollector->ViewProjectionMatrix = editorCamera.GetViewProjectionMatrix();

		DirectX::XMFLOAT4X4 viewMatrix = editorCamera.GetViewMatrix();
		auto viewMatrixMat = DirectX::XMLoadFloat4x4(&viewMatrix);
		DirectX::XMStoreFloat4x4(&m_pRenderCollector->ViewInverseMatrix, DirectX::XMMatrixInverse(nullptr, viewMatrixMat));
	}

	void Renderer::Submit(const Ref<VertexBuffer>& pVertexBuffer, const Ref<IndexBuffer>& pIndexBuffer, const Ref<Shader>& pShader, const DirectX::XMFLOAT4X4& worldTransform)
	{
		m_pRenderCollector->GeometryDrawList.emplace_back(DrawCommand{ pVertexBuffer, pIndexBuffer, pShader, worldTransform });
	}

	void Renderer::Submit(const MeshRendererComponent& meshRendererComponent, const DirectX::XMFLOAT4X4& worldTransform)
	{
		Submit(meshRendererComponent.pVertexBuffer, meshRendererComponent.pIndexBuffer, meshRendererComponent.pShader, worldTransform);
	}

	void Renderer::Submit(const StaticMeshComponent& staticMeshComponent, const DirectX::XMFLOAT4X4& worldTransform)
	{
		for (const auto& pMesh : staticMeshComponent.pMeshes)
		{
			Submit(pMesh->GetVertexBuffer(), pMesh->GetIndexBuffer(), staticMeshComponent.pMaterials[0]->GetShader(), worldTransform);
		}
	}

	void Renderer::Submit(const SkinnedMeshComponent& skinnedMeshComponent, const DirectX::XMFLOAT4X4& worldTransform)
	{
		for (const auto& pMesh : skinnedMeshComponent.pMeshes)
		{
			Submit(pMesh->GetVertexBuffer(), pMesh->GetIndexBuffer(), skinnedMeshComponent.pMaterials[0]->GetShader(), worldTransform);
		}
	}

	void Renderer::Present()
	{
		m_pFinalSceneFramebuffer->Clear();
		m_pFinalSceneFramebuffer->Bind();
		m_pWireframeRasterizerState->Bind();

		for (const DrawCommand& drawCommand : m_pRenderCollector->GeometryDrawList)
		{
			drawCommand.pVertexBuffer->Bind();
			drawCommand.pIndexBuffer->Bind();
			drawCommand.pShader->Bind();

			drawCommand.pShader->UploadMat4("ViewProjection", m_pRenderCollector->ViewProjectionMatrix);
			drawCommand.pShader->UploadMat4("World", drawCommand.WorldTransform);
			drawCommand.pShader->UploadMat4("ViewInverse", m_pRenderCollector->ViewInverseMatrix);

			RenderCommand::DrawIndexed(drawCommand.pIndexBuffer->GetCount(), drawCommand.pShader);
		}

		m_pFinalSceneFramebuffer->Unbind();
	}

	void Renderer::EndScene()
	{
		ClearDrawlist();
	}

	void Renderer::ClearDrawlist()
	{
		m_pRenderCollector->GeometryDrawList.clear();
	}
}

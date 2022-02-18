#pragma once

#include "RenderCommand.h"
#include "Buffer.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "RasterizerState.h"

#include "Camera.h"
#include "EditorCamera.h"

#include "SmileEngine/Scene/Components.h"

namespace Smile
{
	class Framebuffer;

	struct RendererSettings final
	{
		uint32_t Width = 1280;
		uint32_t Height = 720;
	};

	struct DrawCommand final
	{
		Ref<VertexBuffer> pVertexBuffer;
		Ref<IndexBuffer> pIndexBuffer;
		Ref<Shader> pShader;
		DirectX::XMFLOAT4X4 WorldTransform;
	};

	struct RenderCollector final
	{
		DirectX::XMFLOAT4X4 ViewInverseMatrix;
		DirectX::XMFLOAT4X4 ViewProjectionMatrix;

		std::vector<DrawCommand> GeometryDrawList;
	};

	class Renderer final
	{
	public:
		static void Initialize();
		static void ShutDown();

		static void SetSettings(const RendererSettings& settings);

		static void OnWindowResize(uint32_t width, uint32_t height);
		static void ResizeFramebuffer(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera, const DirectX::XMFLOAT4X4& cameraTransform);
		static void BeginScene(const EditorCamera& editorCamera);
		static void EndScene();
		static void Present();

		static void Submit(const Ref<VertexBuffer>& pVertexBuffer, const Ref<IndexBuffer>& pIndexBuffer, const Ref<Shader>& pShader, const DirectX::XMFLOAT4X4& worldTransform);
		static void Submit(const MeshRendererComponent& meshRendererComponent, const DirectX::XMFLOAT4X4& worldTransform);
		static void Submit(const StaticMeshComponent& staticMeshComponent, const DirectX::XMFLOAT4X4& worldTransform);
		static void Submit(const SkinnedMeshComponent& skinnedMeshComponent, const DirectX::XMFLOAT4X4& worldTransform);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static const RendererSettings& GetSettings() { return m_Settings; }
		static void* GetFinalColor() { return m_pFinalSceneFramebuffer->GetColor(0); }

	private:
		static void ClearDrawlist();

	private:
		static RendererSettings m_Settings;
		static RenderCollector* m_pRenderCollector;

		static Ref<Framebuffer> m_pFinalSceneFramebuffer;
		static Ref<RasterizerState> m_pWireframeRasterizerState;
	};
}


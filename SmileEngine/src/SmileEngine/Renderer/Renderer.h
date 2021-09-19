#pragma once

#include "RenderCommand.h"
#include "Buffer.h"
#include "Shader.h"

#include "Camera.h"
#include "SmileEngine/Scene/Components.h"

namespace Smile
{
	class Renderer final
	{
	public:
		static void Initialize();
		static void CleanUp();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera, const DirectX::XMFLOAT4X4& cameraTransform);
		static void EndScene();

		static void Submit(const Ref<VertexBuffer>& pVertexBuffer, const Ref<IndexBuffer>& pIndexBuffer, const Ref<Shader>& pShader, 
			const DirectX::XMFLOAT4X4& worldTransform);
		static void Submit(const MeshRendererComponent& meshRendererComponent, const DirectX::XMFLOAT4X4& worldTransform);
		static void Submit(const StaticMeshComponent& staticMeshComponent, const DirectX::XMFLOAT4X4& worldTransform);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			DirectX::XMFLOAT4X4 ViewInverseMatrix;
			DirectX::XMFLOAT4X4 ViewProjectionMatrix;
		};

		static SceneData* m_pSceneData;
	};
}


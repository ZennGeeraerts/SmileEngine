#pragma once

#include "RenderCommand.h"
#include "Buffer.h"
#include "Shader.h"

#include "OrthographicCamera.h"
#include "SmileEngine/Scene/Components.h"

namespace Smile
{
	class Renderer final
	{
	public:
		static void CleanUp();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(RenderingContext* pRenderingContext, const Ref<VertexBuffer>& pVertexBuffer, const Ref<IndexBuffer>& pIndexBuffer, const Ref<Shader>& pShader, 
			const DirectX::XMFLOAT4X4& worldTransform);

		static void Submit(MeshRendererComponent* pMeshRendererComponent, const DirectX::XMFLOAT4X4& worldTransform);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			DirectX::XMFLOAT4X4 ViewProjectionMatrix;
		};

		static SceneData* m_pSceneData;
	};
}


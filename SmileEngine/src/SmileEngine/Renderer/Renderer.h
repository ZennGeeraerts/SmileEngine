#pragma once

#include "RenderCommand.h"
#include "Buffer.h"
#include "Shader.h"

#include "OrthographicCamera.h"

namespace Smile
{
	class SMILE_API Renderer final
	{
	public:
		static void CleanUp();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(RenderingContext* pRenderingContext, const std::shared_ptr<VertexBuffer>& pVertexBuffer, const std::shared_ptr<IndexBuffer>& pIndexBuffer, const std::shared_ptr<Shader>& pShader, 
			const DirectX::XMFLOAT4X4& worldTransform);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			DirectX::XMFLOAT4X4 ViewProjectionMatrix;
		};

		static SceneData* m_pSceneData;
	};
}


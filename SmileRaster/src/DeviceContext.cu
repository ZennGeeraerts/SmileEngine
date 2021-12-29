#include "DeviceContext.cuh"

#include "Utils.cuh"

// Pipeline
#include "Pipeline/InputAssembler.cu"
#include "Pipeline/VertexShader.cu"
#include "Pipeline/Rasterizer.cu"

#include <iostream>

namespace Smile
{
	namespace Raster
	{
		DeviceContext::DeviceContext(const DeviceContextData& data)
			: m_DCData{ data }
		{
			// Create the screen buffer on the GPU
			size_t size = sizeof(uint8_t) * data.ColorChannelCount * data.Width * data.Height;
			GPU_ERROR_CHECK(cudaMalloc(&d_ScreenBuffer, size));
			GPU_ERROR_CHECK(cudaMemcpy(d_ScreenBuffer, data.pScreenBuffer, size, cudaMemcpyHostToDevice));

			// Create the depth buffer on the GPU
			size = sizeof(float) * data.Width * data.Height;
			GPU_ERROR_CHECK(cudaMalloc(&d_DepthBuffer, size));

			GPU_ERROR_CHECK(cudaMalloc(&d_ShaderData, sizeof(ShaderData)));
		}

		DeviceContext::~DeviceContext()
		{
			GPU_ERROR_CHECK(cudaFree(d_ScreenBuffer));
			GPU_ERROR_CHECK(cudaFree(d_DepthBuffer));

			for (BufferID i{}; i < m_VertexBufferCount; ++i)
				GPU_ERROR_CHECK(cudaFree(m_VertexBuffers[i].d_Vertices));

			for (BufferID i{}; i < m_IndexBufferCount; ++i)
				GPU_ERROR_CHECK(cudaFree(m_IndexBuffers[i].d_Indices));

			GPU_ERROR_CHECK(cudaFree(d_ShaderData));
		}

		BufferID DeviceContext::CreateVertexBuffer(void* pVertices, uint32_t count, uint32_t stride)
		{
			if (m_VertexBufferCount < SMR_MAX_BUFFER_COUNT)
			{
				size_t size = stride * count;
				GPU_ERROR_CHECK(cudaMalloc(&m_VertexBuffers[m_VertexBufferCount].d_Vertices, size));
				GPU_ERROR_CHECK(cudaMemcpy(m_VertexBuffers[m_VertexBufferCount].d_Vertices, pVertices, size, cudaMemcpyHostToDevice));
				m_VertexBuffers[m_VertexBufferCount].Stride = stride;
				m_VertexBuffers[m_VertexBufferCount].Count = count;
				++m_VertexBufferCount;

				return m_VertexBufferCount - 1;
			}

			return SMR_INVALID_BUFFER_ID;
		}

		BufferID DeviceContext::CreateIndexBuffer(uint32_t* pIndices, uint32_t count)
		{
			if (m_IndexBufferCount < SMR_MAX_BUFFER_COUNT)
			{
				size_t size = sizeof(uint32_t) * count;
				GPU_ERROR_CHECK(cudaMalloc(&m_IndexBuffers[m_IndexBufferCount].d_Indices, size));
				GPU_ERROR_CHECK(cudaMemcpy(m_IndexBuffers[m_IndexBufferCount].d_Indices, pIndices, size, cudaMemcpyHostToDevice));
				m_IndexBuffers[m_IndexBufferCount].Count = count;
				++m_IndexBufferCount;

				return m_IndexBufferCount - 1;
			}

			return SMR_INVALID_BUFFER_ID;
		}

		bool DeviceContext::SetVertexBuffer(BufferID id)
		{
			if ((id < static_cast<int>(m_VertexBufferCount)) && (id >= SMR_INVALID_BUFFER_ID))
			{
				m_ActiveVertexBufferID = id;
				return true;
			}

			return false;
		}

		bool DeviceContext::SetIndexBuffer(BufferID id)
		{
			if ((id < static_cast<int>(m_IndexBufferCount)) && (id >= SMR_INVALID_BUFFER_ID))
			{
				m_ActiveIndexBufferID = id;
				return true;
			}

			return false;
		}

		bool bCleared = false;
		void DeviceContext::Clear(const DirectX::XMFLOAT3& clearColor)
		{
			dim3 blockSize = { m_DCData.TileSize, m_DCData.TileSize };
			dim3 gridSize = { static_cast<uint32_t>(ceil(m_DCData.Width / static_cast<float>(m_DCData.TileSize))),
								static_cast<uint32_t>(ceil(m_DCData.Height / static_cast<float>(m_DCData.TileSize))) };

			ClearScreenBufferKernel<<<gridSize, blockSize>>>(d_ScreenBuffer, m_DCData.Width, m_DCData.Height, clearColor, m_DCData.ColorChannelCount);
			ClearDepthBufferKernel<<<gridSize, blockSize>>>(d_DepthBuffer, m_DCData.Width, m_DCData.Height);
			bCleared = true;
		}

		void DeviceContext::DrawIndexed()
		{
			static bool bDraw = true;
			if (!bDraw || !bCleared)
				return;

			if ((m_ActiveVertexBufferID == SMR_INVALID_BUFFER_ID) || (m_ActiveIndexBufferID == SMR_INVALID_BUFFER_ID))
				return;

			VertexBuffer& vertexBuffer = m_VertexBuffers[m_ActiveVertexBufferID];
			IndexBuffer& indexBuffer = m_IndexBuffers[m_ActiveIndexBufferID];

			// Input assembler
			dim3 blockSize = { m_DCData.TileSize, m_DCData.TileSize };
			uint32_t gridSize = static_cast<uint32_t>(ceil(indexBuffer.Count / 3.f / m_DCData.TileSize));
			
			Triangle* d_Triangles = nullptr;
			GPU_ERROR_CHECK(cudaMalloc(&d_Triangles, sizeof(Triangle) * indexBuffer.Count / 3));
			InputAssemblerKernel<<<gridSize, blockSize>>>(d_Triangles, vertexBuffer.d_Vertices, indexBuffer.d_Indices, indexBuffer.Count);

			/*Triangle* pTriangles = (Triangle*)malloc(sizeof(Triangle) * indexBuffer.Count / 3);
			GPU_ERROR_CHECK(cudaMemcpy(pTriangles, d_Triangles, sizeof(Triangle) * indexBuffer.Count / 3, cudaMemcpyDeviceToHost));
			Triangle* pEnd{ pTriangles + indexBuffer.Count / 3 };
			for (Triangle* p{ pTriangles }; p != pEnd; ++p)
			{
				std::cout << (*p).Vertex0.Position.x << "\n";
			}

			for (uint32_t i{}; i < 3; ++i)
			{
				pTriangles[0].Vertices[i].Position.x = (pTriangles[0].Vertices[i].Position.x + 1.0f) * 0.5f * m_DCData.Width;
				pTriangles[0].Vertices[i].Position.y = (1.0f - pTriangles[0].Vertices[i].Position.y) * 0.5f * m_DCData.Height;
			}

			glm::vec2 minPoint{};
			glm::vec2 maxPoint{};
			FindAABB(pTriangles[0], minPoint, maxPoint);
			std::cout << "minPoint: " << minPoint.x << ", " << minPoint.y << '\n';
			std::cout << "maxPoint: " << maxPoint.x << ", " << maxPoint.y << '\n';*/

			// VertexShader
			gridSize = static_cast<uint32_t>(ceil(vertexBuffer.Count / 3.f / m_DCData.TileSize));
			VS_OUTPUT* d_VSOutput = nullptr;
			cudaMalloc(&d_VSOutput, sizeof(VS_OUTPUT) * vertexBuffer.Count);
			VertexShaderKernel<<<gridSize, blockSize>>>(static_cast<VS_INPUT*>(vertexBuffer.d_Vertices), d_VSOutput, d_ShaderData, m_DCData.Width, m_DCData.Height, m_DCData.ColorChannelCount);

			// Rasterization
			gridSize = static_cast<uint32_t>(ceil(indexBuffer.Count / 3.f / m_DCData.TileSize));
			uint32_t* d_Depth = nullptr;
			GPU_ERROR_CHECK(cudaMalloc(&d_Depth, sizeof(uint32_t) * m_DCData.Width * m_DCData.Height * m_DCData.ColorChannelCount));
			RasterizerKernel << <gridSize, blockSize >> > (d_Triangles, indexBuffer.Count / 3, d_DepthBuffer, d_Depth, m_DCData.Width, m_DCData.Height, d_ScreenBuffer, m_DCData.ColorChannelCount);
			
			/*float* pDepthBuffer = (float*)malloc(sizeof(float) * m_DCData.Width * m_DCData.Height);
			GPU_ERROR_CHECK(cudaMemcpy(pDepthBuffer, d_DepthBuffer, sizeof(float) * m_DCData.Width * m_DCData.Height, cudaMemcpyDeviceToHost));
			float* pEnd = pDepthBuffer + m_DCData.Width * m_DCData.Height;
			for (float* p{ pDepthBuffer }; p != pEnd; ++p)
			{
				std::cout << *p << "\n";
			}
			free(pDepthBuffer);*/

			bDraw = false;

			GPU_ERROR_CHECK(cudaFree(d_Triangles));
			GPU_ERROR_CHECK(cudaFree(d_VSOutput));
			GPU_ERROR_CHECK(cudaFree(d_Depth))

			size_t size = sizeof(uint8_t) * m_DCData.ColorChannelCount * m_DCData.Width * m_DCData.Height;
			GPU_ERROR_CHECK(cudaMemcpy(m_DCData.pScreenBuffer, d_ScreenBuffer, size, cudaMemcpyDeviceToHost));
		}

		void DeviceContext::Resize(uint32_t width, uint32_t height, uint8_t* pScreenBuffer)
		{
			// Update device context data
			m_DCData.Width = width;
			m_DCData.Height = height;
			m_DCData.pScreenBuffer = pScreenBuffer;

			// Reallocate the screen buffer in device memory
			GPU_ERROR_CHECK(cudaFree(d_ScreenBuffer));
			size_t size = sizeof(uint8_t) * m_DCData.ColorChannelCount * width * height;
			GPU_ERROR_CHECK(cudaMalloc(&d_ScreenBuffer, size));
			GPU_ERROR_CHECK(cudaMemcpy(d_ScreenBuffer, pScreenBuffer, size, cudaMemcpyHostToDevice));

			// Reallocate the depth buffer in device memory
			GPU_ERROR_CHECK(cudaFree(d_DepthBuffer));
			size = sizeof(float) * width * height;
			GPU_ERROR_CHECK(cudaMalloc(&d_DepthBuffer, size));
		}

		void DeviceContext::SetShaderData(DirectX::XMFLOAT4X4 viewProjection, DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 viewInverse)
		{
			/*d_ShaderData->ViewProjection = viewProjection;
			d_ShaderData->World = world;
			d_ShaderData->ViewInverse = viewInverse;*/
		}
	}
}
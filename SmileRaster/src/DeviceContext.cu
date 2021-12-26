#include "DeviceContext.cuh"

#include "Utils.cuh"

// Pipeline
#include "InputAssembler.cu"
//#include "VertexShader.cu"
//#include "Rasterizer.cu"

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
			GPU_ERROR_CHECK(cudaMemset(d_DepthBuffer, FLT_MAX, size));

			GPU_ERROR_CHECK(cudaMalloc(&d_ShaderData, sizeof(ShaderData)));
		}

		DeviceContext::~DeviceContext()
		{
			GPU_ERROR_CHECK(cudaFree(d_ScreenBuffer));
			GPU_ERROR_CHECK(cudaFree(d_DepthBuffer));

			for (BufferID i{}; i < m_VertexBufferCount; ++i)
				GPU_ERROR_CHECK(cudaFree(&m_VertexBuffers[i].d_Vertices));

			for (BufferID i{}; i < m_IndexBufferCount; ++i)
				GPU_ERROR_CHECK(cudaFree(&m_IndexBuffers[i].d_Indices));

			GPU_ERROR_CHECK(cudaFree(d_ShaderData));
			GPU_ERROR_CHECK(cudaFree(d_Triangles));
		}

		BufferID DeviceContext::CreateVertexBuffer(void* pVertices, uint32_t count, uint32_t stride)
		{
			if (m_VertexBufferCount < SMR_MAX_BUFFER_COUNT)
			{
				size_t size = stride * count;
				GPU_ERROR_CHECK(cudaMalloc(&m_VertexBuffers[m_VertexBufferCount].d_Vertices, size));
				GPU_ERROR_CHECK(cudaMemcpy(m_VertexBuffers[m_VertexBufferCount].d_Vertices, pVertices, size, cudaMemcpyHostToDevice));
				m_VertexBuffers[m_VertexBufferCount].Stride = stride;
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

		void DeviceContext::Clear(const DirectX::XMFLOAT3& clearColor)
		{
			dim3 blockSize = { m_DCData.TileSize, m_DCData.TileSize };
			dim3 gridSize = { static_cast<uint32_t>(ceil(m_DCData.Width / static_cast<float>(m_DCData.TileSize))),
								static_cast<uint32_t>(ceil(m_DCData.Height / static_cast<float>(m_DCData.TileSize))) };

			ClearScreenBufferKernel << <gridSize, blockSize >> > (d_ScreenBuffer, m_DCData.Width, m_DCData.Height, clearColor, m_DCData.ColorChannelCount);

			size_t size = sizeof(uint8_t) * m_DCData.ColorChannelCount * m_DCData.Width * m_DCData.Height;
			GPU_ERROR_CHECK(cudaMemcpy(m_DCData.pScreenBuffer, d_ScreenBuffer, size, cudaMemcpyDeviceToHost));
		}

		void DeviceContext::DrawIndexed()
		{
			static bool bDraw = true;
			if (!bDraw)
				return;

			if ((m_ActiveVertexBufferID == SMR_INVALID_BUFFER_ID) || (m_ActiveIndexBufferID == SMR_INVALID_BUFFER_ID))
				return;

			dim3 blockSize = { m_DCData.TileSize, m_DCData.TileSize };
			dim3 gridSize = { static_cast<uint32_t>(ceil(m_DCData.Width / static_cast<float>(m_DCData.TileSize))),
							static_cast<uint32_t>(ceil(m_DCData.Height / static_cast<float>(m_DCData.TileSize))) };
			
			cudaMalloc(&d_Triangles, sizeof(Triangle) * m_IndexBuffers[m_ActiveIndexBufferID].Count / 3);
			InputAssemblerKernel << <gridSize, blockSize >> > (d_Triangles, m_VertexBuffers[m_ActiveVertexBufferID].d_Vertices, m_IndexBuffers[m_ActiveIndexBufferID].d_Indices, m_IndexBuffers[m_ActiveIndexBufferID].Count);
			
			Triangle* pTriangles = (Triangle*)malloc(sizeof(Triangle) * m_IndexBuffers[m_ActiveIndexBufferID].Count / 3);
			GPU_ERROR_CHECK(cudaMemcpy(pTriangles, d_Triangles, sizeof(Triangle) * m_IndexBuffers[m_ActiveIndexBufferID].Count / 3, cudaMemcpyDeviceToHost));
			Triangle* pEnd = pTriangles + (m_IndexBuffers[m_ActiveIndexBufferID].Count / 3);
			for (Triangle* p = pTriangles; p != pEnd; ++p)
			{
				std::cout << "Triangle " << p - pTriangles << ": (" << p->Vertices[0].Position.x << ", " << p->Vertices[0].Position.y << ", " << p->Vertices[0].Position.z << "), ("
					<< p->Vertices[1].Position.x << ", " << p->Vertices[1].Position.y << ", " << p->Vertices[1].Position.z << "), ("
					<< p->Vertices[2].Position.x << ", " << p->Vertices[2].Position.y << ", " << p->Vertices[2].Position.z << ")\n";
			}

			cudaFree(d_Triangles);
			free(pTriangles);

			bDraw = false;
			
			//VertexShaderKernel<<<gridSize, blockSize>>>(reinterpret_cast<VS_INPUT*>(d_ActiveVertexBuffer), )
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
			GPU_ERROR_CHECK(cudaMemset(d_DepthBuffer, FLT_MAX, size));
		}

		void DeviceContext::SetShaderData(DirectX::XMFLOAT4X4 viewProjection, DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 viewInverse)
		{
			/*d_ShaderData->ViewProjection = viewProjection;
			d_ShaderData->World = world;
			d_ShaderData->ViewInverse = viewInverse;*/
		}
	}
}
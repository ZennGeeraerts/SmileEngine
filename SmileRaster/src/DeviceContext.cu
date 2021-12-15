#include "DeviceContext.cuh"
#include "Utils.cuh"

namespace Smile
{
	namespace Raster
	{
		DeviceContext::DeviceContext(const DeviceContextData& data)
			: m_DCData{ data }
		{
			size_t size = sizeof(uint8_t) * data.ColorChannelCount * data.Width * data.Height;
			GPU_ERROR_CHECK(cudaMalloc(&d_ScreenBuffer, size));
			GPU_ERROR_CHECK(cudaMemcpy(d_ScreenBuffer, data.pScreenBuffer, size, cudaMemcpyHostToDevice));
		}

		DeviceContext::~DeviceContext()
		{
			GPU_ERROR_CHECK(cudaFree(d_ScreenBuffer));

			for (BufferID i{}; i < m_VertexBufferCount; ++i)
				GPU_ERROR_CHECK(cudaFree(&d_VertexBuffers[i]));

			for (BufferID i{}; i < m_IndexBufferCount; ++i)
				GPU_ERROR_CHECK(cudaFree(&d_IndexBuffers[i]));
		}

		BufferID DeviceContext::CreateVertexBuffer(void* pVertices, uint32_t count, uint32_t stride)
		{
			if (m_VertexBufferCount < SMR_MAX_BUFFER_COUNT)
			{
				size_t size = stride * count;
				GPU_ERROR_CHECK(cudaMalloc(&d_VertexBuffers[m_VertexBufferCount], size));
				GPU_ERROR_CHECK(cudaMemcpy(d_VertexBuffers[m_VertexBufferCount], pVertices, size, cudaMemcpyHostToDevice));
				++m_VertexBufferCount;

				return m_VertexBufferCount - 1;
			}

			return SMR_INVALID_BUFFER_ID;
		}

		bool DeviceContext::SetVertexBuffer(BufferID id)
		{
			if (id == SMR_INVALID_BUFFER_ID)
			{
				d_ActiveVertexBuffer = nullptr;
				return true;
			}

			if (id < static_cast<int>(m_VertexBufferCount))
			{
				d_ActiveVertexBuffer = d_VertexBuffers[id];
				return true;
			}

			return false;
		}

		BufferID DeviceContext::CreateIndexBuffer(uint32_t* pIndices, uint32_t count)
		{
			if (m_IndexBufferCount < SMR_MAX_BUFFER_COUNT)
			{
				size_t size = sizeof(uint32_t) * count;
				GPU_ERROR_CHECK(cudaMalloc(&d_IndexBuffers[m_IndexBufferCount], size));
				GPU_ERROR_CHECK(cudaMemcpy(d_IndexBuffers[m_IndexBufferCount], pIndices, size, cudaMemcpyHostToDevice));
				++m_IndexBufferCount;

				return m_IndexBufferCount - 1;
			}

			return SMR_INVALID_BUFFER_ID;
		}

		bool DeviceContext::SetIndexBuffer(BufferID id)
		{
			if (id == SMR_INVALID_BUFFER_ID)
			{
				d_ActiveIndexBuffer = nullptr;
				return true;
			}

			if (id < static_cast<int>(m_IndexBufferCount))
			{
				d_ActiveIndexBuffer = d_IndexBuffers[id];
				return true;
			}

			return false;
		}

		void DeviceContext::Clear(const DirectX::XMFLOAT3& clearColor)
		{
			dim3 blockSize = { m_DCData.TileSize, m_DCData.TileSize };
			dim3 gridSize = { static_cast<uint32_t>(ceil(m_DCData.Width / static_cast<float>(m_DCData.TileSize))),
								static_cast<uint32_t>(ceil(m_DCData.Height / static_cast<float>(m_DCData.TileSize))) };

			ClearScreenBufferKernel << <gridSize, blockSize >> > (d_ScreenBuffer, m_DCData.Width, m_DCData.Height, clearColor);
			//GPU_ERROR_CHECK(cudaDeviceSynchronize());

			size_t size = sizeof(uint8_t) * m_DCData.ColorChannelCount * m_DCData.Width * m_DCData.Height;
			GPU_ERROR_CHECK(cudaMemcpy(m_DCData.pScreenBuffer, d_ScreenBuffer, size, cudaMemcpyDeviceToHost));
		}

		void DeviceContext::DrawIndexed()
		{
			dim3 blockSize = { m_DCData.TileSize, m_DCData.TileSize };
			dim3 gridSize = { static_cast<uint32_t>(ceil(m_DCData.Width / static_cast<float>(m_DCData.TileSize))),
							static_cast<uint32_t>(ceil(m_DCData.Height / static_cast<float>(m_DCData.TileSize))) };

		}
	}
}
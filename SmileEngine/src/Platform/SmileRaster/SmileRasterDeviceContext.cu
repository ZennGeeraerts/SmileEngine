#include "SmileRasterDeviceContext.cuh"
//#include <cuda_runtime.h>
//#include "SmileRasterRasterizer.cu"
#include "SmileEngine/Core/Logger.h"

namespace Smile
{
	SmileRasterDeviceContext::SmileRasterDeviceContext(const SmileRasterDeviceContextData& data)
		: m_DCData{ data }
	{
		size_t size = sizeof(uint8_t) * data.ColorChannelCount * data.Width * data.Height;
		GPU_ERROR_CHECK(cudaMalloc(&d_ScreenBuffer, size));
		GPU_ERROR_CHECK(cudaMemcpy(d_ScreenBuffer, data.pScreenBuffer, size, cudaMemcpyHostToDevice));
	}

	SmileRasterDeviceContext::~SmileRasterDeviceContext()
	{
		GPU_ERROR_CHECK(cudaFree(d_ScreenBuffer));

		for (SMBufferID i{}; i < m_VertexBufferCount; ++i)
			GPU_ERROR_CHECK(cudaFree(&d_VertexBuffers[i]));

		for (SMBufferID i{}; i < m_IndexBufferCount; ++i)
			GPU_ERROR_CHECK(cudaFree(&d_IndexBuffers[i]));
	}

	SMBufferID SmileRasterDeviceContext::CreateVertexBuffer(void* pVertices, uint32_t count, uint32_t stride)
	{
		if (m_VertexBufferCount < MAX_BUFFER_COUNT)
		{
			size_t size = stride * count;
			GPU_ERROR_CHECK(cudaMalloc(&d_VertexBuffers[m_VertexBufferCount], size));
			GPU_ERROR_CHECK(cudaMemcpy(d_VertexBuffers[m_VertexBufferCount], pVertices, size, cudaMemcpyHostToDevice));
			++m_VertexBufferCount;

			return m_VertexBufferCount - 1;
		}

		return INVALID_SM_BUFFER_ID;
	}

	bool SmileRasterDeviceContext::SetVertexBuffer(SMBufferID id)
	{
		if (id == INVALID_SM_BUFFER_ID)
		{
			d_ActiveVertexBuffer = nullptr;
			return true;
		}

		if (id < m_VertexBufferCount)
		{
			d_ActiveVertexBuffer = d_VertexBuffers[id];
			return true;
		}

		return false;
	}

	SMBufferID SmileRasterDeviceContext::CreateIndexBuffer(uint32_t* pIndices, uint32_t count)
	{
		if (m_IndexBufferCount < MAX_BUFFER_COUNT)
		{
			size_t size = sizeof(uint32_t) * count;
			GPU_ERROR_CHECK(cudaMalloc(&d_IndexBuffers[m_IndexBufferCount], size));
			GPU_ERROR_CHECK(cudaMemcpy(d_IndexBuffers[m_IndexBufferCount], pIndices, size, cudaMemcpyHostToDevice));
			++m_IndexBufferCount;

			return m_IndexBufferCount - 1;
		}

		return INVALID_SM_BUFFER_ID;
	}

	bool SmileRasterDeviceContext::SetIndexBuffer(SMBufferID id)
	{
		if (id == INVALID_SM_BUFFER_ID)
		{
			d_ActiveIndexBuffer = nullptr;
			return true;
		}

		if (id < m_IndexBufferCount)
		{
			d_ActiveIndexBuffer = d_IndexBuffers[id];
			return true;
		}

		return false;
	}

	__global__ void ClearScreenBufferKernel(uint8_t* pScreenBuffer, uint32_t width, uint32_t height, DirectX::XMFLOAT3 clearColor)
	{
		uint32_t pixelX = (blockIdx.x * blockDim.x) + threadIdx.x;
		uint32_t pixelY = (blockIdx.y * blockDim.y) + threadIdx.y;
		uint32_t bufferIndex = (pixelY * width + pixelX) * 3;

		if (bufferIndex < (width * height * 3))
		{
			pScreenBuffer[bufferIndex] = clearColor.z * 255.f;
			pScreenBuffer[bufferIndex + 1] = clearColor.y * 255.f;
			pScreenBuffer[bufferIndex + 2] = clearColor.x * 255.f;
		}
	}

	void SmileRasterDeviceContext::Clear(const DirectX::XMFLOAT3& clearColor)
	{
		dim3 blockSize = { m_DCData.TileSize, m_DCData.TileSize };
		dim3 gridSize = { static_cast<uint32_t>(ceil(m_DCData.Width / static_cast<float>(m_DCData.TileSize))),
							static_cast<uint32_t>(ceil(m_DCData.Height / static_cast<float>(m_DCData.TileSize))) };

		ClearScreenBufferKernel << <gridSize, blockSize >> > (d_ScreenBuffer, m_DCData.Width, m_DCData.Height, clearColor);
		//GPU_ERROR_CHECK(cudaDeviceSynchronize());

		size_t size = sizeof(uint8_t) * m_DCData.ColorChannelCount * m_DCData.Width * m_DCData.Height;
		GPU_ERROR_CHECK(cudaMemcpy(m_DCData.pScreenBuffer, d_ScreenBuffer, size, cudaMemcpyDeviceToHost));
	}

	void SmileRasterDeviceContext::DrawIndexed()
	{
		dim3 blockSize = { m_DCData.TileSize, m_DCData.TileSize };
		dim3 gridSize = { static_cast<uint32_t>(ceil(m_DCData.Width / static_cast<float>(m_DCData.TileSize))),
						static_cast<uint32_t>(ceil(m_DCData.Height / static_cast<float>(m_DCData.TileSize))) };

	}
}
#include "SmileRasterDeviceContext.cuh"
#include <cuda_runtime.h>

namespace Smile
{
	SmileRasterDeviceContext::SmileRasterDeviceContext(uint32_t width, uint32_t height)
		: h_Width{ width }
		, h_Height{ height }
	{

	}

	SmileRasterDeviceContext::~SmileRasterDeviceContext()
	{
		cudaFree(d_VertexBuffer);
		cudaFree(d_IndexBuffer);
	}

	void SmileRasterDeviceContext::SetVertexBuffer(void* pVertexBuffer, uint32_t count, uint32_t stride)
	{
		cudaFree(d_VertexBuffer);

		size_t size = stride * count;
		cudaMalloc(&d_VertexBuffer, size);
		cudaMemcpy(d_VertexBuffer, pVertexBuffer, size, cudaMemcpyHostToDevice);

		h_VertexBufferCount = count;
		h_VertexBufferStride = stride;
	}

	void SmileRasterDeviceContext::SetIndexBuffer(uint32_t* pIndexBuffer, uint32_t count)
	{
		cudaFree(d_IndexBuffer);

		size_t size = count * sizeof(uint32_t);
		cudaMalloc(&d_IndexBuffer, size);
		cudaMemcpy(d_IndexBuffer, pIndexBuffer, size, cudaMemcpyHostToDevice);

		h_IndexBufferCount = count;
	}

	void SmileRasterDeviceContext::DrawIndexed()
	{
		uint32_t tileSize = 8;
		dim3 blockSize = { tileSize, tileSize };
		dim3 gridSize = { h_Width / tileSize, h_Height / tileSize };


	}
}
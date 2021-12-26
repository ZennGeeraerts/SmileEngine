#include <curand_kernel.h>
#include <stdio.h>
#include <stdint.h>
#include <DirectXMath.h>

namespace Smile
{
    namespace Raster
    {
		#define GPU_ERROR_CHECK(ans) { GPUAssert((ans), __FILE__, __LINE__); }
		inline void GPUAssert(cudaError_t error, const char* pFile, int line, bool bAbort = true)
		{
			if (error != cudaSuccess)
			{
				fprintf(stderr, "GPUAssert: %s %s %d\n", cudaGetErrorString(error), pFile, line);
				if (bAbort)
					exit(error);
			}
		}

		__global__ inline void ClearScreenBufferKernel(uint8_t* pScreenBuffer, uint32_t width, uint32_t height, DirectX::XMFLOAT3 clearColor, uint8_t colorChannelCount)
		{
			uint32_t pixelX = (blockIdx.x * blockDim.x) + threadIdx.x;
			uint32_t pixelY = (blockIdx.y * blockDim.y) + threadIdx.y;
			uint32_t bufferIndex = (pixelY * width + pixelX) * colorChannelCount;

			if (bufferIndex < (width * height * colorChannelCount))
			{
				pScreenBuffer[bufferIndex] = static_cast<uint8_t>(clearColor.z * 255.f);
				pScreenBuffer[bufferIndex + 1] = static_cast<uint8_t>(clearColor.y * 255.f);
				pScreenBuffer[bufferIndex + 2] = static_cast<uint8_t>(clearColor.x * 255.f);
			}
		}
    }
}
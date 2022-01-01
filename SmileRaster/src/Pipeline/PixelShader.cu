#include "../DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void PixelShaderKernel(const VS_OUTPUT* pInput, uint8_t* pScreenBuffer, float* pDepthBuffer, uint32_t width, uint32_t height, uint8_t colorChannelCount)
		{
			uint32_t pixelX = (blockIdx.x * blockDim.x) + threadIdx.x;
			uint32_t pixelY = (blockIdx.y * blockDim.y) + threadIdx.y;
			uint32_t pixelIndex = pixelY * width + pixelX;

			if (pixelIndex < (width * height))
			{
				if (pDepthBuffer[pixelIndex] < FLT_MAX)
				{
					pScreenBuffer[pixelIndex * colorChannelCount] = pInput[pixelIndex].Color.b * 255.f;
					pScreenBuffer[pixelIndex * colorChannelCount + 1] = pInput[pixelIndex].Color.g * 255.f;
					pScreenBuffer[pixelIndex * colorChannelCount + 2] = pInput[pixelIndex].Color.r * 255.f;
				}
			}
		}
	}
}
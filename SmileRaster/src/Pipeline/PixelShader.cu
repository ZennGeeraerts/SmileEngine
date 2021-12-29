#include "../DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void PixelShaderKernel(const VS_OUTPUT* pInput, uint8_t* pScreenbuffer, uint32_t width, uint32_t height, uint8_t colorChannelCount)
		{
			uint32_t pixelX = (blockIdx.x * blockDim.x) + threadIdx.x;
			uint32_t pixelY = (blockIdx.y * blockDim.y) + threadIdx.y;
			uint32_t bufferIndex = (pixelY * width + pixelX) * colorChannelCount;

			if (bufferIndex < (width * height * colorChannelCount))
			{
				const VS_OUTPUT& input = pInput[bufferIndex];

				//pScreenbuffer[bufferIndex];
			}
		}
	}
}
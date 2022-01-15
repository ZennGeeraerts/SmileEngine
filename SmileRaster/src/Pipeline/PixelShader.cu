#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void PixelShaderKernel(Framebuffer framebuffer)
		{
			uint32_t pixelX = (blockIdx.x * blockDim.x) + threadIdx.x;
			uint32_t pixelY = (blockIdx.y * blockDim.y) + threadIdx.y;
			uint32_t pixelIndex = pixelY * framebuffer.Width + pixelX;

			if (pixelIndex < (framebuffer.Width * framebuffer.Height))
			{
				if (framebuffer.d_DepthBuffer[pixelIndex] < FLT_MAX)
				{
					framebuffer.d_ColorBuffer[pixelIndex * framebuffer.ColorChannelCount] = framebuffer.d_PixelData[pixelIndex].Color.b * 255.f;
					framebuffer.d_ColorBuffer[pixelIndex * framebuffer.ColorChannelCount + 1] = framebuffer.d_PixelData[pixelIndex].Color.g * 255.f;
					framebuffer.d_ColorBuffer[pixelIndex * framebuffer.ColorChannelCount + 2] = framebuffer.d_PixelData[pixelIndex].Color.r * 255.f;
				}
			}
		}
	}
}
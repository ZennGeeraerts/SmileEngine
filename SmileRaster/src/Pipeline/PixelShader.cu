#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__device__ glm::vec3 Texture2DSample(const Texture2D& texture2D, const glm::vec2& texCoord)
		{
			uint32_t x = static_cast<uint32_t>(texCoord.x * texture2D.Width);
			uint32_t y = static_cast<uint32_t>(texCoord.y * texture2D.Height);
			uint32_t pixelIndex{ x + y * texture2D.Width };

			return glm::vec3{ texture2D.d_Pixels[pixelIndex * 4], texture2D.d_Pixels[pixelIndex * 4 + 1], texture2D.d_Pixels[pixelIndex * 4 + 2] } / 255.f;
		}

		__global__ void PixelShaderKernel(Framebuffer framebuffer, Texture2D albedoMap)
		{
			uint32_t pixelX = (blockIdx.x * blockDim.x) + threadIdx.x;
			uint32_t pixelY = (blockIdx.y * blockDim.y) + threadIdx.y;
			uint32_t pixelIndex = pixelY * framebuffer.Width + pixelX;

			if (pixelIndex < (framebuffer.Width * framebuffer.Height))
			{
				if (framebuffer.d_DepthBuffer[pixelIndex] < FLT_MAX)
				{
					glm::vec3 lightDirection{ 0.577f, -0.577f, 0.577f };
					glm::vec3 color = Texture2DSample(albedoMap, framebuffer.d_PixelData[pixelIndex].TexCoord);

					float diffuseStrength = glm::dot(framebuffer.d_PixelData[pixelIndex].Normal, -lightDirection);
					diffuseStrength = diffuseStrength * 0.5f + 0.5f;
					diffuseStrength = glm::clamp(diffuseStrength, 0.f, 1.f);
					color *= diffuseStrength;

					framebuffer.d_ColorBuffer[pixelIndex * framebuffer.ColorChannelCount] = color.b * 255.f;
					framebuffer.d_ColorBuffer[pixelIndex * framebuffer.ColorChannelCount + 1] = color.g * 255.f;
					framebuffer.d_ColorBuffer[pixelIndex * framebuffer.ColorChannelCount + 2] = color.r * 255.f;
				}
			}
		}
	}
}
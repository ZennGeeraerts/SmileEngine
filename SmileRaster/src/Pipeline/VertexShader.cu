#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void VertexShaderKernel(const VertexShaderInput* pInput, VertexShaderOutput* pOutput, uint32_t vertexBufferCount, glm::mat4 viewProjection, glm::mat4 world)
		{
			uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;

			if (index < vertexBufferCount)
			{
				const VertexShaderInput& input = pInput[index];
				VertexShaderOutput& output = pOutput[index];

				glm::mat4 worldViewProjectionMatrix = viewProjection * world;

				output.Position = worldViewProjectionMatrix * glm::vec4{ input.Position, 1.f };
				output.Normal = input.Normal;
				output.Tangent = input.Tangent;
				output.TexCoord = input.TexCoord;

				output.Position.x /= output.Position.w;
				output.Position.y /= output.Position.w;
				output.Position.z /= output.Position.w;
			}
		}
	}
}
#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void VertexShaderKernel(const VS_INPUT* pInput, VS_OUTPUT* pOutput, ShaderData shaderData, uint32_t vertexBufferCount)
		{
			uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;

			if (index < vertexBufferCount)
			{
				const VS_INPUT& input = pInput[index];
				VS_OUTPUT& output = pOutput[index];

				glm::mat4 worldViewProjectionMatrix = shaderData.ViewProjection * shaderData.World;

				output.Position = worldViewProjectionMatrix * glm::vec4{ input.Position, 1.f };
				output.Color = input.Color;

				output.Position.x /= output.Position.w;
				output.Position.y /= output.Position.w;
				output.Position.z /= output.Position.w;
			}
		}
	}
}
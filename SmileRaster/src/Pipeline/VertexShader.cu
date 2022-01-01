#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void VertexShaderKernel(const VS_INPUT* pInput, VS_OUTPUT* pOutput, ShaderData* pShaderData, uint32_t vertexBufferCount)
		{
			uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;

			if (index < vertexBufferCount)
			{
				const VS_INPUT& input = pInput[index];
				VS_OUTPUT& output = pOutput[index];

				glm::vec4 position{ input.Position, 1.0f };

				/*position.x /= position.w;
				position.y /= position.w;
				position.z /= position.w;*/

				output.Position = { input.Position, 1.f };
				output.Color = input.Color;
				//output.Color = input.Color;
			}
		}
	}
}
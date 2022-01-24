#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void InputAssemblerKernel(const void* d_Vertices, uint32_t vertexBufferCount, VertexShaderInput* d_VertexShaderInput, uint32_t vertexStride)
		{
			uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;

			if (index < vertexBufferCount)
			{
				const void* d_DataLocation = (uint8_t*)d_Vertices + vertexStride * index;
				memcpy(&d_VertexShaderInput[index], d_DataLocation, min(vertexStride, static_cast<uint32_t>(sizeof(VertexShaderInput))));
			}
		}
	}
}
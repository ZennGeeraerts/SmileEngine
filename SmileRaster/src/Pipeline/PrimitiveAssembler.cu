#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void PrimitiveAssemblerKernel(Triangle* pPrimitives, uint32_t primitiveCount, VertexShaderOutput* pVertexOutput, uint32_t* pIndices)
		{
			uint32_t triangleIndex = (blockIdx.x * blockDim.x) + threadIdx.x;

			if (triangleIndex < primitiveCount)
			{
                uint32_t vertexIndex0 = pIndices[3 * triangleIndex];
                uint32_t vertexIndex1 = pIndices[3 * triangleIndex + 1];
                uint32_t vertexIndex2 = pIndices[3 * triangleIndex + 2];

				pPrimitives[triangleIndex].Vertices[0] = pVertexOutput[vertexIndex0];
				pPrimitives[triangleIndex].Vertices[1] = pVertexOutput[vertexIndex1];
				pPrimitives[triangleIndex].Vertices[2] = pVertexOutput[vertexIndex2];
			}
		}
	}
}
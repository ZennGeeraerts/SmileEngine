#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void InputAssemblerKernel(Triangle* pTriangles, void* pVertices, uint32_t* pIndices, uint32_t indexCount)
		{
			uint32_t triangleIndex = (blockIdx.x * blockDim.x) + threadIdx.x;
			VS_INPUT* pVertexInputBuffer = static_cast<VS_INPUT*>(pVertices);
			const uint32_t triangleCount = indexCount / 3;

			if (triangleIndex < triangleCount)
			{
                uint32_t vertexIndex0 = pIndices[3 * triangleIndex];
                uint32_t vertexIndex1 = pIndices[3 * triangleIndex + 1];
                uint32_t vertexIndex2 = pIndices[3 * triangleIndex + 2];

				pTriangles[triangleIndex].Vertices[0] = pVertexInputBuffer[vertexIndex0];
				pTriangles[triangleIndex].Vertices[1] = pVertexInputBuffer[vertexIndex1];
				pTriangles[triangleIndex].Vertices[2] = pVertexInputBuffer[vertexIndex2];
			}
		}
	}
}
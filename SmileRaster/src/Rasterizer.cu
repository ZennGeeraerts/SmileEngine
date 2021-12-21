#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdint.h>

namespace Smile
{
	namespace Raster
	{
		__global__ void VertexShaderKernel(void* pVertexBuffer)
		{
			uint32_t index = (blockIdx.x * blockDim.x) + threadIdx.x;
		}
	}
}